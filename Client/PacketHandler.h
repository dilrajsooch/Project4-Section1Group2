#pragma once
#include "Packet.h"

static class PacketHandler
{
	static Packet CreateTextPostPacket(char* text, int size, int roomNumber)
	{
		Packet packet;
		packet.SetType(TEXT_POST);
		packet.SetBody(text, size);
		packet.SetRoomNumber(roomNumber);
		return packet;
	}

	static Packet CreateImagePostPacket(char* text, int size, Image image, int roomNumber)
	{
		Packet packet;
		packet.SetType(IMAGE_POST);
		packet.SetBody(text, size, image);
		packet.SetRoomNumber(roomNumber);
		return packet;
	}
};