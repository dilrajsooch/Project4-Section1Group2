#pragma once
#include "string"
#include <raygui.h>
#include "User.h"

using namespace std;


class Post
{
private:
	int id;
	int userid;
	int roomNumber;
	bool isImage = false;
	string text;
	Image image;

public:
	Post(int roomNumber, string text, int uid)
	{
		this->roomNumber = roomNumber;
		this->text = text;
		userid = uid;
	}

	Post(int roomNumber,Image image, int uid)
	{
		isImage = true;
		this->roomNumber = roomNumber;
		this->image = image;
		userid = uid;
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

	int GetUserID()
	{
		return userid;
	}

	void SetUserID(int userId)
	{
		this->userid = userId;
	}

};