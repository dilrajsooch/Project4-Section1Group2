#pragma once
#include <memory>
#include <iostream>
#include <fstream>
#include <raygui.h>



enum PacketType 
{
	TEXT_POST,
	IMAGE_POST,
	COMMAND,
	AUTH_REQUEST
};

class Packet
{
	struct Header
	{
		unsigned char postTextSize; // Size of the post text
		unsigned int imageSize; // Size of the image
		PacketType type; // The type of packet
	} Head;
	struct Body
	{
		char* postText;
		Image image;
	} Body;

	const int BASEPKTSIZE = sizeof(Header);

	char* TxBuffer;

public:
	
	Packet() : TxBuffer(nullptr)
	{
		memset(&Head, 0, sizeof(Head));
		Head.postTextSize = 0;
		Head.imageSize = 0;
		Head.type = TEXT_POST;
		Body.postText = nullptr;
		Body.image = { 0 };
	}

	Packet(char* src)
	{
		memcpy(&Head, src, sizeof(Head));
		
		if (Head.postTextSize > 0)
		{
			Body.postText = new char[Head.postTextSize + 1];
			memcpy(&Body, src + sizeof(Head), Head.postTextSize);
		}

		if (Head.imageSize > 0)
		{
			unsigned char* imgData = new unsigned char[Head.imageSize];
			memcpy(imgData, src + sizeof(Head) + Head.postTextSize, Head.imageSize);

			Body.image = LoadImageFromMemory("PNG", imgData, Head.imageSize);

			delete[] imgData;
		}

	}

	void SetType(PacketType type)
	{
		Head.type = type;
	}

	void SetBody(char* srcData, int Size)
	{

		if (Body.postText)
		{
			Body.postText = nullptr;
			delete[] Body.postText;
		}

		Body.postText = new char[Size + 1];

		memcpy(Body.postText, srcData, Size);
		Body.postText[Size] = '\0';

		Head.postTextSize = Size;
	}

	void SetBody(char* srcData, int txtSize, Image image, unsigned int imageSize)
	{
		txtSize++;

		if (Body.postText)
		{
			delete[] Body.postText;
		}

		Body.postText = new char[txtSize];

		memcpy(Body.postText, srcData, txtSize);
		Body.postText[txtSize - 1] = '\0';
		Body.image = image;

		Head.postTextSize = txtSize;
		Head.imageSize = imageSize;
	}

	char* SerializeData()
	{
		// Clear buffer
		if (TxBuffer)
		{
			delete[] TxBuffer;
		}

		int totalSize = GetSize();
		
		// Creates new buffer
		TxBuffer = new char[totalSize];

		
		memcpy(TxBuffer, &Head, sizeof(Head));
		memcpy(TxBuffer + sizeof(Head), Body.postText, Head.postTextSize);
		memcpy(TxBuffer + sizeof(Head) + Head.postTextSize, &Body.image, Head.imageSize);

		return TxBuffer;
	}

	char* GetText()
	{
		return Body.postText;
	}

private:
	int GetSize()
	{
		return BASEPKTSIZE + Head.imageSize + Head.postTextSize;
	}
};