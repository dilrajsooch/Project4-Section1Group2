#pragma once
#include <string>
#include "raygui.h"

using namespace std;

class User
{
	private:
		int id;
		string userName;
		Image profilePic;
	public:

		User()
		{
			this->id = 0;
			this->userName = "";
		}

		User(int id, string userName)
		{
			this->id = id;
			this->userName = userName;
		}

		User(int id, string userName, Image profilePic)
		{
			this->id = id;
			this->userName = userName;
			this->profilePic = profilePic;
		}

		void SetId(int id)
		{
			this->id = id;
		}

		int GetId()
		{
			return id;
		}

		void SetUsername(string userName)
		{
			this->userName = userName;
		}

		string GetUsername()
		{
			return userName;
		}

		void SetProfilePic(Image profilePic)
		{
			this->profilePic = profilePic;
		}

		Image GetProfilePic()
		{
			return profilePic;
		}

		static User MainUser;

};