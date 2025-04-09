#pragma once
#include "string"
#include "vector"
#include "Post.h"

class ChatRoom
{
private:
	int roomNumber;
	// The posts of the chatroom
	vector<Post> posts;
	// The name of the chatroom
	string name;

public:

	ChatRoom(string name, int id)
	{
		this->name = name;
		roomNumber = id;
	}
	/// <summary>
	/// Sets the name of the chat room
	/// </summary>
	/// <param name="name">The name</param>
	void SetName(string name)
	{
		this->name = name;
	}

	/// <summary>
	/// Gets the name of the chat room
	/// </summary>
	/// <returns>string</returns>
	string GetName()
	{
		return name;
	}

	/// <summary>
	/// Gets a post of the chat room using the index
	/// </summary>
	/// <param name="index">The index of the post</param>
	/// <returns>Post</returns>
	Post GetPostByIndex(int index)
	{
		return posts.at(index);
	}

	/// <summary>
	/// The amount of posts of the room
	/// </summary>
	/// <returns>the count</returns>
	int GetPostCount()
	{
		return posts.size();
	}

	/// <summary>
	/// Adds a post to the chatroom
	/// </summary>
	/// <param name="post">The post to be added</param>
	void AddPost(Post post)
	{
		posts.push_back(post);
	}

	/// <summary>
	/// Sets the room's number
	/// </summary>
	/// <param name="roomNumber">The number to set</param>
	void SetRoomNumber(int roomNumber)
	{
		this->roomNumber = roomNumber;
	}

	/// <summary>
	/// Gets the room number
	/// </summary>
	/// <returns>The room number</returns>
	int GetRoomNumber()
	{
		return roomNumber;
	}
};