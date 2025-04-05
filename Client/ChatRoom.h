#pragma once
#include "string"
#include "vector"
#include "Post.h"

class ChatRoom
{
private:
	// The posts of the chatroom
	vector<Post> posts;
	// The name of the chatroom
	string name;

public:
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
	/// Adds a post to the chatroom
	/// </summary>
	/// <param name="post">The post to be added</param>
	void AddPost(Post post)
	{
		posts.push_back(post);
	}

};