#pragma once
#include "string"
#include <raygui.h>


using namespace std;


class Post
{
private:
	int id;
	int pid;
	int roomNumber;
	string text;
	Image image;
	string author;

public:
	Post(int roomNumber, string text, Image image, string author)
	{
		this->roomNumber = roomNumber;
		this->text = text;
		this->image = image;
		this->author = author;
	}

	int GetRoomNumber()
	{
		return roomNumber;
	}

	string GetText()
	{
		return text;
	}

	Image GetImage()
	{
		return image;
	}

	string GetAuthor()
	{
		return author;
	}
};