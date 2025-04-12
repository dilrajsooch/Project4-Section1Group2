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
			// First receive the header to determine the total packet size
			char headerBuffer[sizeof(Packet::Header)];
			if (recv(clientSocket, headerBuffer, sizeof(Packet::Header), 0) < 0)
			{
				cerr << "ERROR: Failed to receive packet header" << endl;
				Packet failedPacket;
				failedPacket.SetBody("-1", 2);
				return failedPacket;
			}

			// Parse the header to get the total size
			Packet::Header header;
			memcpy(&header, headerBuffer, sizeof(Packet::Header));
			
			int totalSize = sizeof(Packet::Header) + header.postTextSize + header.imageSize;
			
			// Allocate a buffer for the entire packet
			char* buffer = new char[totalSize];
			
			// Copy the header we already received
			memcpy(buffer, headerBuffer, sizeof(Packet::Header));
			
			// Receive the rest of the packet
			int bytesReceived = 0;
			int remainingBytes = totalSize - sizeof(Packet::Header);
			
			while (bytesReceived < remainingBytes)
			{
				int result = recv(clientSocket, buffer + sizeof(Packet::Header) + bytesReceived, 
					remainingBytes - bytesReceived, 0);
				
				if (result <= 0)
				{
					cerr << "ERROR: Failed to receive packet body" << endl;
					delete[] buffer;
					Packet failedPacket;
					failedPacket.SetBody("-1", 2);
					return failedPacket;
				}
				
				bytesReceived += result;
			}
			
			// Create a new packet from the complete data
			Packet newPacket(buffer);
			delete[] buffer;
			return newPacket;
		}
};