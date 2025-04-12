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
	Post(int roomNumber, string text, int uid, int pid = 0)
	{
		this->roomNumber = roomNumber;
		this->text = text;
		userid = uid;
		id = pid;
		
		// Initialize image to a safe empty state
		image = {0};
	}

	Post(int roomNumber, Image image, int uid, int pid = 0)
	{
		isImage = true;
		this->roomNumber = roomNumber;
		this->image = image;
		userid = uid;
		id = pid;
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

	int GetID()
	{
		return id;
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