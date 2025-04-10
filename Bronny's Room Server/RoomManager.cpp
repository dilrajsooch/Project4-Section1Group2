#include "RoomManager.h"

RoomManager& RoomManager::getInstance() {
    static RoomManager instance;
    return instance;
}

RoomManager::RoomManager() { }

RoomManager::~RoomManager() { }

void RoomManager::addMessage(int roomId, const Message& msg) {
    std::lock_guard<std::mutex> lock(mMessagesMutex);
    mMessages[roomId].push_back(msg);
}

void RoomManager::addUser(int roomId, const User& user) {
    std::lock_guard<std::mutex> lock(mUsersMutex);
    mUsers[roomId].push_back(user);
}

std::vector<Message> RoomManager::getMessages(int roomId) {
    std::lock_guard<std::mutex> lock(mMessagesMutex);
    if (mMessages.find(roomId) != mMessages.end()) {
        return mMessages[roomId];
    }
    return std::vector<Message>();
}

std::vector<User> RoomManager::getUsers(int roomId) {
    std::lock_guard<std::mutex> lock(mUsersMutex);
    if (mUsers.find(roomId) != mUsers.end()) {
        return mUsers[roomId];
    }
    return std::vector<User>();
}

bool RoomManager::deleteMessage(int roomId, size_t index) {
    std::lock_guard<std::mutex> lock(mMessagesMutex);
    auto roomIt = mMessages.find(roomId);
    if (roomIt != mMessages.end()) {
        auto& messages = roomIt->second;
        if (index < messages.size()) {
            messages.erase(messages.begin() + index);
            return true;
        }
    }
    return false;
}
