#include "RoomManager.h"
#include <cstring>
#include <algorithm>
#include <sstream>
#include <iostream>

RoomManager& RoomManager::getInstance() {
    static RoomManager instance;
    return instance;
}

RoomManager::RoomManager() {}

RoomManager::~RoomManager() {
    for (auto& roomPair : mMessages) {
        for (auto& post : roomPair.second) {
            delete[] post.Data;
        }
    }
}

int RoomManager::addMessage(int roomId, int userId, std::string msg) {
    std::lock_guard<std::mutex> lock(mMessagesMutex);

    // Create a Room key from the provided roomId.
    Room key;
    key.id = roomId;
    key.name = "";  // You can adjust this if you maintain room names

    std::vector<Post>& posts = mMessages[key];

    Post post;
    post.IsImage = false;
    post.RoomId = roomId;
    post.UserId = userId;
    
    int length = static_cast<int>(msg.size());
    post.Data = new char[length + 1];
    post.size = length;
    std::copy(msg.begin(), msg.end(), post.Data);
    post.Data[length] = '\0';

    post.PostId = static_cast<int>(posts.size()) + 1;
    posts.push_back(post);

    return post.PostId;
}

int RoomManager::addImage(int roomId, int userId, char* img, int size) {
    std::lock_guard<std::mutex> lock(mMessagesMutex);

    Room key;
    key.id = roomId;
    key.name = "";

    std::vector<Post>& posts = mMessages[key];

    Post post;
    post.IsImage = true;
    post.RoomId = roomId;
    post.UserId = userId;
    post.size = size;

    // Don't add null terminator for binary data
    post.Data = new char[size];
    std::copy(img, img + size, post.Data);

    post.PostId = static_cast<int>(posts.size()) + 1;
    posts.push_back(post);

    return post.PostId;
}

void RoomManager::addUser(int roomId, const User& user) {
    std::lock_guard<std::mutex> lock(mUsersMutex);

    // Build the Room key
    Room key;
    key.id = roomId;
    key.name = "";

    mUsers[key].push_back(user);
}

int RoomManager::addRoom(std::string name) {
    std::lock_guard<std::mutex> lockMessages(mMessagesMutex);
    std::lock_guard<std::mutex> lockUsers(mUsersMutex);

    //Set id to 1 or latest id
    int newId = 1;
    if (!mMessages.empty()) {
        newId = mMessages.rbegin()->first.id + 1;
    }

    //Create room
    Room room;
    room.id = newId;
    room.name = name;

    //Initalize Room
    mMessages[room] = std::vector<Post>();
    mUsers[room] = std::vector<User>();

    return newId;
}


std::string RoomManager::getRooms() {
    std::lock_guard<std::mutex> lock(mMessagesMutex);

    std::ostringstream oss;
    bool first = true;
    for (const auto& pair : mMessages) {
        if (!first) {
            oss << "|";
        }
        //Format is "id:name|id:name"
        oss << pair.first.id << ":" << pair.first.name;
        first = false;
    }
    return oss.str();
}

std::string RoomManager::getPosts(int roomId) {
    std::lock_guard<std::mutex> lock(mMessagesMutex);

    Room key;
    key.id = roomId;
    key.name = "";

    auto it = mMessages.find(key);
    if (it != mMessages.end()) {
        return SerializePosts(it->second);
    }
    return "";
}

std::vector<User> RoomManager::getUsers(int roomId) {
    std::lock_guard<std::mutex> lock(mUsersMutex);

    Room key;
    key.id = roomId;
    key.name = "";

    auto it = mUsers.find(key);
    if (it != mUsers.end()) {
        return it->second;
    }
    return std::vector<User>();
}

bool RoomManager::deletePost(int roomId, int messageId, int userId) {
    std::lock_guard<std::mutex> lock(mMessagesMutex);

    Room key;
    key.id = roomId;
    key.name = "";

    auto roomIt = mMessages.find(key);
    if (roomIt != mMessages.end()) {
        auto& posts = roomIt->second;
        for (auto iter = posts.begin(); iter != posts.end(); ++iter) {
            if (iter->PostId == messageId && iter->UserId == userId) {
                delete[] iter->Data;
                posts.erase(iter);
                return true;
            }
        }
    }
    return false;
}

std::string RoomManager::SerializePosts(const std::vector<Post>& posts) {
    std::ostringstream oss;
    for (size_t i = 0; i < posts.size(); i++) {
        oss << posts[i].PostId << ":" << posts[i].UserId << ":"  << posts[i].IsImage << ":" << posts[i].size << ":";
        
        // For image posts, don't include the binary data in the string
        if (!posts[i].IsImage) {
            oss << posts[i].Data;
        } else {
            // For images, just include a placeholder
            oss << "[IMAGE_DATA]";
        }
        
        if (i != posts.size() - 1) {
            oss << "|";
        }
    }
    return oss.str();
}

std::vector<Post> RoomManager::getImagePosts(int roomId) {
    std::lock_guard<std::mutex> lock(mMessagesMutex);

    Room key;
    key.id = roomId;
    key.name = "";

    auto it = mMessages.find(key);
    if (it != mMessages.end()) {
        std::vector<Post> imagePosts;
        for (const auto& post : it->second) {
            if (post.IsImage) {
                imagePosts.push_back(post);
            }
        }
        return imagePosts;
    }
    return std::vector<Post>();
}
