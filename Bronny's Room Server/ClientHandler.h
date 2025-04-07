#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS // for gethostbyname, etc.
#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <thread>
#include <vector>
#include <atomic>

#pragma comment(lib, "ws2_32.lib")


#include "ClientHandler.h"
#include "include/Packet.h"
#include "include/Logging.h"
#include "include/CredentialManager.h"
#include "include/ServerState.h"

void SendLargeImage(SOCKET clientSocket, const std::string& clientIP, const std::string& filename);
void ClientHandler(SOCKET clientSocket, sockaddr_in clientAddr);