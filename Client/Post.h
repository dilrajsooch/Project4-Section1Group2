#pragma once
#include "string"
#include <raygui.h>
#include "User.h"

using namespace std;


class Post
{
private:
	int id;
	int pid;
	int roomNumber;
	string text;
	Image image;
	User author;

public:
	Post(int roomNumber, string text, User author)
	{
		this->roomNumber = roomNumber;
		this->text = text;
		this->image = image;
		this->author = author;
	}
	Post(int roomNumber, string text, Image image, User author)
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

	User GetAuthor()
	{
		return author;
	}
};