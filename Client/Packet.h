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
		int imageSize; // Size of the image
		PacketType type; // The type of packet
		int roomNumber; // The room to send this post to
	} Head;
	struct Body
	{
		char* postText;
		unsigned char* imageData;
	} Body;

	const int BASEPKTSIZE = sizeof(Header);

	char* TxBuffer;

public:
	
	Packet() : TxBuffer(nullptr)
	{
		memset(&Head, 0, sizeof(Head));
		Head.postTextSize = 0;
		Head.imageSize = 0;
		Head.roomNumber = 0;
		Head.type = TEXT_POST;
		Body.postText = nullptr;
		Body.imageData = nullptr;
	}

	Packet(char* src)
	{
		memcpy(&Head, src, sizeof(Head));
		
		if (Head.postTextSize > 0)
		{
			Body.postText = new char[Head.postTextSize];
			memcpy(Body.postText, src + sizeof(Head), Head.postTextSize);
		}

		if (Head.imageSize > 0)
		{
			Body.imageData = new unsigned char[Head.imageSize];
			memcpy(Body.imageData, src + sizeof(Head) + Head.postTextSize, Head.imageSize);
		}
	}
	
	void SetRoomNumber(int number)
	{
		Head.roomNumber = number;
	}

	void SetType(PacketType type)
	{
		Head.type = type;
	}

	void SetBody(char* srcData, int Size)
	{

		if (Body.postText)
		{
			delete[] Body.postText;
		}

		Body.postText = new char[Size + 1];

		memcpy(Body.postText, srcData, Size);
		Body.postText[Size] = '\0';

		Head.postTextSize = Size + 1;
	}

	void SetBody(char* srcData, int txtSize, Image image)
	{

		if (Body.postText)
		{
			delete[] Body.postText;
		}

		Body.postText = new char[txtSize + 1];

		memcpy(Body.postText, srcData, txtSize);
		Body.postText[txtSize] = '\0';

		if (Body.imageData)
		{
			delete[] Body.imageData;
		}
		int exportedSize = 0;
		Body.imageData = ExportImageToMemory(image, ".png", &exportedSize);

		if (!Body.imageData)
		{
			Head.imageSize = 0;
			return;
		}

		Head.postTextSize = txtSize;
		Head.imageSize = exportedSize;
	}

	Image GetImage()
	{
		return LoadImageFromMemory(".png", Body.imageData, Head.imageSize);
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
		if (Body.postText)
		{
			memcpy(TxBuffer + sizeof(Head), Body.postText, Head.postTextSize);
		}
		if(Body.imageData)
		{ 
			memcpy(TxBuffer + sizeof(Head) + Head.postTextSize, Body.imageData, Head.imageSize);
		}

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