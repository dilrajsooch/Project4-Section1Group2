#pragma once

#define NOMINMAX
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include "Packet.h"
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

class CSocket
{
	private:
		const string IP = "172.16.5.12";
		const int PORT = 25565;
		int clientSocket;
		sockaddr_in SvrAddr;
	public:
		CSocket()
		{
			// For windows, remove when moving to linux
			WSAData wsaData;
			if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
			{
				cout << "ERROR: Failed to start WSA" << endl;
				WSACleanup();
				return;
			}

			// Create Socket
			// Create Client Socket
			clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

			if (clientSocket == -1)
			{
				cout << "ERROR: Failed to create socket" << endl;
				return;
			}

			Connect(IP, PORT);
		}
		void Connect(const string& ip, int port)
		{
			SvrAddr.sin_family = AF_INET;
			SvrAddr.sin_port = htons(port);

			if (inet_pton(AF_INET, ip.c_str(), &SvrAddr.sin_addr) <= 0)
			{
				cout << "ERROR: Invalid IP address format." << endl;
				closesocket(clientSocket);
				WSACleanup();
				exit(1);
			}

			if ((connect(clientSocket, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr))) == -1)
			{
				closesocket(clientSocket);
				WSACleanup();
				cerr << "ERROR: Failed to connect to server." << endl;
				exit(1);
				return;
			}
		}

		Packet SendPacket(Packet packet)
		{
		
			if (send(clientSocket, packet.SerializeData(), packet.GetSize(), 0) == -1)
			{
				cerr << "ERROR: Failed to send packet." << endl;
				closesocket(clientSocket);
				WSACleanup();
				return nullptr;
			}
			return RecievePacket();
		}

		Packet RecievePacket()
		{
			char buffer[1024]; // Not sure what the size should be
			if (recv(clientSocket, buffer, sizeof(buffer), 0) < 0)
			{
				cerr << "ERROR: Failed to recieve packet" << endl;
				return nullptr;

			}
			Packet newPacket(buffer);
			return newPacket;
		}
};