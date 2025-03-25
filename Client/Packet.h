#pragma once
#include <memory>
#include <iostream>
#include <fstream>

const int BASEPKTSIZE; // TBD

enum PacketType {
	TEXT_POST,
	IMAGE_POST,
	COMMAND,
	AUTH_REQUEST
};

class Packet
{
	struct Header
	{
		unsigned char dataSize; // Size of the body
		PacketType type; // The type of packet
	} Head;
	char* Data;

	char* TxBuffer;
};