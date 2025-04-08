#pragma once
#define WIN32_LEAN_AND_MEAN   // Exclude rarely-used Windows headers
#define NOGDI             // All GDI defines and routines
#define NOUSER            // All USER defines and routines
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
		
		int clientSocket;
		sockaddr_in SvrAddr;
		static CSocket* _instance;

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

		}

	public:
		
		int Connect(const string& ip, int port)
		{
			SvrAddr.sin_family = AF_INET;
			SvrAddr.sin_port = htons(port);

			if (inet_pton(AF_INET, ip.c_str(), &SvrAddr.sin_addr) <= 0)
			{
				cout << "ERROR: Invalid IP address format." << endl;
				closesocket(clientSocket);
				WSACleanup();
				return -2;
			}

			if ((connect(clientSocket, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr))) == -1)
			{
				closesocket(clientSocket);
				WSACleanup();
				cerr << "ERROR: Failed to connect to server." << endl;
				return -1;
			}
			return 1;
		}

		static CSocket* GetInstance()
		{
			if (_instance == nullptr)
			{
				_instance = new CSocket();
			}
			return _instance;
		}

		Packet SendPacket(Packet packet)
		{
		
			if (send(clientSocket, packet.SerializeData(), packet.GetSize(), 0) == -1)
			{
				cerr << "ERROR: Failed to send packet." << endl;
				closesocket(clientSocket);
				WSACleanup();
				Packet failedPacket;
				failedPacket.SetBody("-1", 2);
				return failedPacket;
			}
			return RecievePacket();
		}

		Packet RecievePacket()
		{
			char buffer[1024]; // Not sure what the size should be
			if (recv(clientSocket, buffer, sizeof(buffer), 0) < 0)
			{
				cerr << "ERROR: Failed to recieve packet" << endl;
				Packet failedPacket;
				failedPacket.SetBody("-1", 2);
				return failedPacket;

			}
			Packet newPacket(buffer);
			return newPacket;
		}
};