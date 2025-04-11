#define _CRT_SECURE_NO_WARNINGS

#define WIN32_LEAN_AND_MEAN   // Exclude rarely-used Windows headers
#define NOGDI                 // All GDI defines and routines
#define NOUSER                // All USER defines and routines
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

void ClientHandler(SOCKET clientSocket, sockaddr_in clientAddr);

void RunNetworkServer() {
    //Init
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed with error: " << result << std::endl;
        return;
    }

    //Create Socket
    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET) {
        std::cerr << "socket() failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return;
    }

    //Bind Socket
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    hint.sin_addr.s_addr = INADDR_ANY;

    if (bind(listenSocket, (sockaddr*)&hint, sizeof(hint)) == SOCKET_ERROR) {
        std::cerr << "bind() failed: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return;
    }

    //Listen
    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "listen() failed: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return;
    }

    //Server is up!
    SetServerState(ServerState::RUNNING);
    std::cout << "Server is now RUNNING on port 54000...\n";

    //Accept clients
    while (GetServerState() == ServerState::RUNNING) {
        sockaddr_in clientAddr;
        int clientSize = sizeof(clientAddr);
        SOCKET clientSocket = accept(listenSocket, (sockaddr*)&clientAddr, &clientSize);
        if (clientSocket == INVALID_SOCKET) {
            //If the server is shutting down, accept might fail.
            if (GetServerState() == ServerState::DEINITIALIZING) {
                break;
            }
            std::cerr << "accept() failed: " << WSAGetLastError() << std::endl;
            continue;
        }

        //Create Client Handlet thread
        std::thread t(ClientHandler, clientSocket, clientAddr);
        t.detach();
    }

    //Cleanup
    closesocket(listenSocket);
    WSACleanup();
}

int main() {
    std::thread networkThread(RunNetworkServer);

    ServerGUI gui;
    gui.Init(600, 600);
    //Blocking
    gui.RunLoop();

    //After run loop
    SetServerState(ServerState::DEINITIALIZING);

    //Wait for thread to finish
    if (networkThread.joinable()) {
        networkThread.join();
    }

    SetServerState(ServerState::TERMINATED);
    std::cout << "Server has terminated.\n";

    return 0;
}
