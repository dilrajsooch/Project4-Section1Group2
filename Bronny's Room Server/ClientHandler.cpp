#include "include/ClientHandler.h"

// Example session state for each client
enum class ClientSessionState {
    AUTH_PENDING,
    AUTHENTICATED
};


void ClientHandler(SOCKET clientSocket, sockaddr_in clientAddr)
{
    // Convert IP to string
    char ipStr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(clientAddr.sin_addr), ipStr, INET_ADDRSTRLEN);
    std::string clientIP = ipStr;
    int clientPort = ntohs(clientAddr.sin_port);


    GlobalDataModel::getInstance().UserConnected();
    std::cout << "Client connected from " << clientIP << ":" << clientPort << "\n";
    

    ClientSessionState sessionState = ClientSessionState::AUTH_PENDING;

    bool keepRunning = true;
    while (keepRunning)
    {
        // (1) Receive the Header
        // We assume your Packet class has a 'struct Header' you can read
        char headerBuf[sizeof(Packet::Header)];
        int bytesReceived = recv(clientSocket, headerBuf, sizeof(headerBuf), 0);

        if (bytesReceived <= 0)
        {
            std::cout << "Client " << clientIP << ":" << clientPort << " disconnected.\n";
            break;
        }

        // (2) Parse the header to know how many more bytes to read
        Packet::Header head;
        memcpy(&head, headerBuf, sizeof(Packet::Header));

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
        std::vector<char> fullPacket(sizeof(Packet::Header) + bodySize);
        memcpy(fullPacket.data(), &head, sizeof(Packet::Header));
        if (bodySize > 0)
            memcpy(fullPacket.data() + sizeof(Packet::Header), bodyBuf.data(), bodySize);

        // (4) Construct the Packet
        Packet pkt(fullPacket.data());

        // (5) Log the incoming packet
        Logger::getInstance().LogPacket(INCOMING_PACKET, clientIP, pkt);

        // (6) Process the Packet
        switch (head.type)
        {
        case AUTH_REQUEST:
        {
            if (sessionState == ClientSessionState::AUTH_PENDING)
            {
                std::string creds = pkt.GetText();
                // Parse user/pass out of this
                auto pos = creds.find("|");
                std::string user = creds.substr(0, pos);
                std::string pass = creds.substr(pos + 1);
                
                bool result = ValidateCredentials(user, pass);
                std::string userId;


                if (result == true)
                {
                    sessionState = ClientSessionState::AUTHENTICATED;
                    std::cout << "Client " << clientIP << " authenticated successfully\n";
                    userId = GetAccountID(user);
                }
                else
                {
                    std::cout << "Client " << clientIP << " failed authentication\n";
                    userId = "-1";
                }

                // Create return packet
                Packet packet;
                packet.SetType(Packet::AUTH_RESPONSE);
                packet.SetBody(userId.c_str(), (int)userId.size());

                // Log return packet
                Logger::getInstance().LogPacket(OUTGOING_PACKET, clientIP, packet);

                //Send return packet
                send(clientSocket, packet.SerializeData(), packet.GetSize(), 0);
            }
            else
            {
                // Already authenticated?
            }
        } break;

        case GET_ROOMS:
        {
            if (sessionState != ClientSessionState::AUTHENTICATED)
            {
                std::cout << "Ignoring command from unauthenticated client\n";
                continue;
            }

        } break;

        case ADD_ROOM:
        {
            if (sessionState != ClientSessionState::AUTHENTICATED)
            {
                std::cout << "Ignoring command from unauthenticated client\n";
                continue;
            }
        } break;

        case ADD_POST:
        {
            if (sessionState != ClientSessionState::AUTHENTICATED)
            {
                std::cout << "Ignoring command from unauthenticated client\n";
                continue;
            }
        } break;

        case DELETE_POST:
        {
            if (sessionState != ClientSessionState::AUTHENTICATED)
            {
                std::cout << "Ignoring command from unauthenticated client\n";
                continue;
            }
        } break;

        case GET_POST:
        {
            if (sessionState != ClientSessionState::AUTHENTICATED)
            {
                std::cout << "Ignoring command from unauthenticated client\n";
                continue;
            }
        } break;

        default:
            std::cout << "Unknown packet type from client.\n";
        } break;

        // (7) Check global server state
        if (GetServerState() == ServerState::DEINITIALIZING)
        {
            std::cout << "Server shutting down. Closing client socket.\n";
            keepRunning = false;
        }

        
    }

    closesocket(clientSocket);
    std::cout << "Client thread exiting for " << clientIP << ":" << clientPort << "\n";
    GlobalDataModel::getInstance().UserDisconnected();
}
