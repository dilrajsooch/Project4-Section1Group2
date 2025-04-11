#pragma once
#define WIN32_LEAN_AND_MEAN   // Exclude rarely-used Windows headers
#define NOGDI             // All GDI defines and routines
#define NOUSER            // All USER defines and routines
#define NOMINMAX
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <atomic>
#include <vector>
#include <map>

#pragma comment(lib, "ws2_32.lib")


#include "ClientHandler.h"
#include "include/Logging.h"
#include "include/CredentialManager.h"
#include "include/ServerState.h"
#include "include/GlobalDataModel.h"

void ClientHandler(SOCKET clientSocket, sockaddr_in clientAddr);