#pragma once
#define WIN32_LEAN_AND_MEAN   // Exclude rarely-used Windows headers
#define NOGDI             // All GDI defines and routines
#define NOUSER            // All USER defines and routines
#define NOMINMAX
#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>
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
			// First, read the header:
			int bytesReceived = 0;
			const int headerSize = sizeof(Packet::Header);
			char headerBuffer[headerSize];
			while (bytesReceived < headerSize) {
				int res = recv(clientSocket, headerBuffer + bytesReceived, headerSize - bytesReceived, 0);
				// Check for errors or connection closure
				if (res <= 0) { /* handle error */ }
				bytesReceived += res;
			}

			// Now extract total size using the header fields (text and image sizes)
			Packet::Header header;
			memcpy(&header, headerBuffer, sizeof(Packet::Header));
			int totalSize = sizeof(Packet::Header) + header.postTextSize + header.imageSize;

			// Allocate a buffer for the rest of the packet:
			std::vector<char> packetBuffer(totalSize);
			// Copy the already-read header into packetBuffer.
			memcpy(packetBuffer.data(), headerBuffer, sizeof(Packet::Header));

			// Now, read the remaining data:
			bytesReceived = sizeof(Packet::Header);
			while (bytesReceived < totalSize) {
				int res = recv(clientSocket, packetBuffer.data() + bytesReceived, totalSize - bytesReceived, 0);
				if (res <= 0) { /* handle error */ }
				bytesReceived += res;
			}

			// Now packetBuffer holds the full packet.
			Packet newPacket(packetBuffer.data());
			return newPacket;
		}
};