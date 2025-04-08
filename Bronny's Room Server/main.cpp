#define WIN32_LEAN_AND_MEAN   // Exclude rarely-used Windows headers
#define NOGDI             // All GDI defines and routines
#define NOUSER            // All USER defines and routines
#define NOMINMAX
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <thread>
#include <vector>
#include <atomic>

#pragma comment(lib, "ws2_32.lib")

#include "include/ServerState.h"
#include "include/GlobalDataModel.h"
#include "include/ServerGui.h"

// Global or static variables
static std::atomic_bool g_isRunning(true);

// Forward declare
void ClientHandler(SOCKET clientSocket, sockaddr_in clientAddr);


int main()
{
    // 1) Initialize WinSock
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed with error: " << result << std::endl;
        return 1;
    }

    // 2) Create a socket (IPv4, TCP)
    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET) {
        std::cerr << "socket() failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // 3) Bind the socket to a port, e.g. 54000
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    hint.sin_addr.s_addr = INADDR_ANY;

    if (bind(listenSocket, (sockaddr*)&hint, sizeof(hint)) == SOCKET_ERROR) {
        std::cerr << "bind() failed: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    // 4) Listen
    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "listen() failed: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    // Start GUI Thread
    ServerGUI gui;
    gui.Init(200, 200);
    gui.RunLoop();

    // We are up -> set server state to RUNNING
    SetServerState(ServerState::RUNNING);
    std::cout << "Server is now RUNNING on port 54000...\n";

    // 5) Accept multiple clients in a loop
    while (GetServerState() == ServerState::RUNNING) {
        sockaddr_in clientAddr;
        int clientSize = sizeof(clientAddr);

        SOCKET clientSocket = accept(listenSocket, (sockaddr*)&clientAddr, &clientSize);
        if (clientSocket == INVALID_SOCKET) {
            // If the server is shutting down, accept might fail
            if (GetServerState() == ServerState::DEINITIALIZING) {
                break; // exit the accept loop
            }
            std::cerr << "accept() failed: " << WSAGetLastError() << std::endl;
            continue;
        }

        // 6) Spin up a thread to handle the new client
        std::thread t(ClientHandler, clientSocket, clientAddr);
        t.detach();
    }

    // If we get here, we might be DEINITIALIZING...
    // 7) Clean up
    closesocket(listenSocket);
    WSACleanup();

    SetServerState(ServerState::TERMINATED);
    std::cout << "Server has terminated.\n";

    return 0;
}

