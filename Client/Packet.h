#pragma once
#include <memory>
#include <iostream>
#include <fstream>

class Packet
{
private:
	char* TxBuffer; // The send buffer

public:

	enum PacketType
	{
		GET_ROOMS,
		ADD_ROOM,
		ADD_POST,
		DELETE_POST,
		GET_POST,
		AUTH_REQUEST,
		AUTH_RESPONSE
	};

	struct Header
	{
		unsigned char postTextSize; // Size of the post text
		int imageSize; // Size of the image
		PacketType type; // The type of packet
		int roomNumber; // The room to send this post to
		int userId;
		bool isImage;
	} Head;

	struct Body
	{
		char* postText; // The text of the post
		char* imageData; // The data of the image
	} Body;

	const int BASEPKTSIZE = sizeof(Header);

	/// <summary>
	/// Default Constructor
	/// </summary>
	Packet() : TxBuffer(nullptr)
	{
		// Safe Empty State
		memset(&Head, 0, sizeof(Head));
		Head.postTextSize = 0;
		Head.imageSize = 0;
		Head.roomNumber = 0;
		Head.type = ADD_POST;
		Body.postText = nullptr;
		Body.imageData = nullptr;
	}

	/// <summary>
	/// Create a packet based on a deserialized packet
	/// </summary>
	/// <param name="src">The byte array of the incomming packet</param>
	Packet(char* src) : TxBuffer(nullptr)
	{
		if (src == NULL)
		{
			return;
		}
		memcpy(&Head, src, sizeof(Head));
		
		if (Head.postTextSize > 0)
		{
			Body.postText = new char[Head.postTextSize];
			memcpy(Body.postText, src + sizeof(Head), Head.postTextSize);
		}

		if (Head.imageSize > 0)
		{
			std::cout << "Image size: " << Head.imageSize << std::endl;
			Body.imageData = new char[Head.imageSize];
			memcpy(Body.imageData, src + sizeof(Head) + Head.postTextSize, Head.imageSize);
		}
	}
	
	/// <summary>
	/// Sets the destination room number
	/// </summary>
	/// <param name="number">The number of the room</param>
	void SetRoomNumber(int number)
	{
		Head.roomNumber = number;
	}

	/// <summary>
	/// Sets the type of the packet
	/// </summary>
	/// <param name="type">The PacketType</param>
	void SetType(PacketType type)
	{
		Head.type = type;
	}

	void SetUserId(int id) {
		Head.userId = id;
	}

	/// <summary>
	/// Sets the text of the body
	/// </summary>
	/// <param name="srcData">The text to be sent</param>
	/// <param name="Size">The size of the text</param>
	void SetBody(const char* srcData, int Size, bool isImage = false)
	{
		if (!isImage)
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
		else
		{
			if (Body.imageData)
			{
				delete[] Body.imageData;
			}
			Body.imageData = new char[Size];
			memcpy(Body.imageData, srcData, Size);

			Head.imageSize = Size;
		}

	}

	/// <summary>
	/// Gets the image from the packet
	/// </summary>
	/// <returns>Image</returns>
	char* GetImage()
	{
		return Body.imageData;
	}

	int GetUserId() {
		return Head.userId;
	}

	/// <summary>
	/// Serializes the packet to be sent over the net
	/// </summary>
	/// <returns>Byte array</returns>
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
	/// <summary>
	/// Gets the text of the body
	/// </summary>
	/// <returns>Character array</returns>
	const char* GetText()
	{
		return Body.postText;
	}

	PacketType GetType() {
		return Head.type;
	}

	/// <summary>
	/// Gets the total size of the packet
	/// </summary>
	/// <returns>The total size of the packet</returns>
	int GetSize()
	{
		return BASEPKTSIZE + Head.imageSize + Head.postTextSize;
	}
};