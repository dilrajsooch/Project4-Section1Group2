// ClientHandler.cpp

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <thread>
#include <vector>
#include "ClientHandler.h"
#include "Packet.h"
#include "Logging.h"
#include "CredentialManager.h"
#include "ServerState.h"

#pragma comment(lib, "ws2_32.lib")

// Example session state for each client
enum class ClientSessionState {
    AUTH_PENDING,
    AUTHENTICATED
};

void SendLargeImage(SOCKET clientSocket, const std::string& clientIP, const std::string& filename)
{
    // 1) Open the file in binary mode
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open())
    {
        std::cout << "Could not open image file: " << filename << "\n";
        return;
    }

    // 2) Get file size
    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // 3) Read file data into buffer
    std::vector<char> fileData(fileSize);
    if (!file.read(fileData.data(), fileSize))
    {
        std::cout << "Error reading file data.\n";
        return;
    }

    // 4) Build a Packet
    Packet pkt;
    pkt.SetType(IMAGE_POST);

    // If your Packet supports something like:
    // pkt.SetBody(char* text, int textSize, Image image, unsigned int imageSize);
    // But here we only have raw bytes. Possibly you need a specialized overload that sets
    // the raw data as an "image". For demonstration, we just call a function that
    // stores the bytes:
    pkt.SetBody((char*)"", 0, fileData.data(), (unsigned int)fileSize);

    // 5) Serialize
    char* serialized = pkt.SerializeData();
    int totalSize = pkt.GetSize();

    // 6) Send in a loop until all bytes are sent
    int bytesSent = 0;
    while (bytesSent < totalSize)
    {
        int chunk = send(clientSocket, serialized + bytesSent, totalSize - bytesSent, 0);
        if (chunk == SOCKET_ERROR)
        {
            std::cout << "Send error: " << WSAGetLastError() << "\n";
            break;
        }
        bytesSent += chunk;
    }

    // Log it
    LogPacket("TX", clientIP, pkt);

    std::cout << "Finished sending image to " << clientIP << ". (" << bytesSent << " bytes)\n";

    // If needed, free resources, e.g. if Packet doesn’t manage that memory for you:
    // delete[] serialized;
}

void ClientHandler(SOCKET clientSocket, sockaddr_in clientAddr)
{
    // Convert IP to string
    char ipStr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(clientAddr.sin_addr), ipStr, INET_ADDRSTRLEN);
    std::string clientIP = ipStr;
    int clientPort = ntohs(clientAddr.sin_port);

    std::cout << "Client connected from " << clientIP << ":" << clientPort << "\n";

    ClientSessionState sessionState = ClientSessionState::AUTH_PENDING;

    bool keepRunning = true;
    while (keepRunning)
    {
        // (1) Receive the Header
        // We assume your Packet class has a 'struct Header' you can read
        char headerBuf[sizeof(Header)];
        int bytesReceived = recv(clientSocket, headerBuf, sizeof(headerBuf), 0);

        if (bytesReceived <= 0)
        {
            std::cout << "Client " << clientIP << ":" << clientPort << " disconnected.\n";
            break;
        }

        // (2) Parse the header to know how many more bytes to read
        Header head;
        memcpy(&head, headerBuf, sizeof(Header));

        // (3) Receive the body, if any
        int bodySize = head.postTextSize + head.imageSize; // from your Packet struct
        std::vector<char> bodyBuf(bodySize);
        if (bodySize > 0)
        {
            int bodyReceived = 0;
            while (bodyReceived < bodySize)
            {
                int chunk = recv(clientSocket, bodyBuf.data() + bodyReceived, bodySize - bodyReceived, 0);
                if (chunk <= 0)
                {
                    std::cout << "Client " << clientIP << ":" << clientPort << " disconnected.\n";
                    keepRunning = false;
                    break;
                }
                bodyReceived += chunk;
            }
            if (!keepRunning) break; // break outer loop if disconnect in partial recv
        }

        // Combine header + body
        std::vector<char> fullPacket(sizeof(Header) + bodySize);
        memcpy(fullPacket.data(), &head, sizeof(Header));
        if (bodySize > 0)
            memcpy(fullPacket.data() + sizeof(Header), bodyBuf.data(), bodySize);

        // (4) Construct the Packet
        Packet pkt(fullPacket.data());

        // (5) Log the incoming packet
        LogPacket("RX", clientIP, pkt);

        // (6) Process the Packet
        switch (head.type)
        {
        case AUTH_REQUEST:
        {
            if (sessionState == ClientSessionState::AUTH_PENDING)
            {
                std::string creds = pkt.GetText(); // e.g. "User:Pass"
                // Parse user/pass out of this
                auto pos = creds.find(":");
                std::string user = creds.substr(0, pos);
                std::string pass = creds.substr(pos + 1);

                if (ValidateCredentials(user, pass))
                {
                    sessionState = ClientSessionState::AUTHENTICATED;
                    std::cout << "Client " << clientIP << " authenticated successfully\n";
                    // Possibly send a response packet to confirm
                }
                else
                {
                    std::cout << "Client " << clientIP << " failed authentication\n";
                    // Possibly send error packet, or close connection
                }
            }
            else
            {
                // Already authenticated?
            }
        } break;

        case COMMAND:
        {
            if (sessionState != ClientSessionState::AUTHENTICATED)
            {
                std::cout << "Ignoring command from unauthenticated client\n";
                // Optionally respond with error
                continue;
            }

            // For example, check if command is "SHUTDOWN" or "REQUEST_IMAGE"
            std::string cmd = pkt.GetText();
            if (cmd == "SHUTDOWN")
            {
                // Possibly set global state to DEINITIALIZING
                SetServerState(ServerState::DEINITIALIZING);
                keepRunning = false;
            }
            else if (cmd.find("REQUEST_IMAGE") == 0)
            {
                // e.g. "REQUEST_IMAGE test.png"
                // We'll do the large data send (see next section)
                std::string filename = cmd.substr(cmd.find(" ") + 1);
                SendLargeImage(clientSocket, clientIP, filename);
            }
        } break;

        case TEXT_POST:
        {
            // Chat message or blog post
            if (sessionState == ClientSessionState::AUTHENTICATED)
            {
                std::string message = pkt.GetText();
                std::cout << "Client posted text: " << message << "\n";
                // Possibly store or broadcast
            }
        } break;

        case IMAGE_POST:
        {
            // Client is sending an image to server
            if (sessionState == ClientSessionState::AUTHENTICATED)
            {
                // We already have the data in pkt
                // Possibly store it or forward to other clients
            }
        } break;

        case LARGE_IMAGE_POST:
        {
            if (sessionState == ClientSessionState::AUTHENTICATED)
            {
                SendLargeImage(clientSocket, clientIP, pkt.GetText());
            }
            break;
        }

        default:
            std::cout << "Unknown packet type from client.\n";
            break;
        }

        // (7) Check global server state
        if (GetServerState() == ServerState::DEINITIALIZING)
        {
            std::cout << "Server shutting down. Closing client socket.\n";
            keepRunning = false;
        }

        
    }

    closesocket(clientSocket);
    std::cout << "Client thread exiting for " << clientIP << ":" << clientPort << "\n";
}
