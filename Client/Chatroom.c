#include "ChatRoom.h"

using std::string;
using std::vector;

void ChatRoom::SetName(string name)
{
    this->name = name;
}

string ChatRoom::GetName()
{
    return name;
}

Post ChatRoom::GetPostByIndex(int index)
{
    return posts.at(index);
}

void ChatRoom::AddPost(Post post)
{
    posts.push_back(post);
}
