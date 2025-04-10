#pragma once
#include <string>
#include "raygui.h"

using namespace std;

class User
{
	private:
		int id;
	public:

		User()
		{
			this->id = 0;
		}

		User(int id)
		{
			this->id = id;
		}

		void SetId(int id)
		{
			this->id = id;
		}

		int GetId()
		{
			return id;
		}

		static User MainUser;

};