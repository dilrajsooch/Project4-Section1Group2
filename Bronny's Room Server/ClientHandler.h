#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS // for gethostbyname, etc.
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include "Packet.h"
void SendLargeImage(SOCKET clientSocket, const std::string& clientIP, const std::string& filename);
void ClientHandler(SOCKET clientSocket, sockaddr_in clientAddr);