#pragma once

#include <map>
#include <vector>
#include <mutex>
#include <string>

struct Message {
    std::string message;
};

struct User {
    int id;
    int socket;
};

class RoomManager {
public:
    static RoomManager& getInstance();

    RoomManager(const RoomManager&) = delete;
    RoomManager& operator=(const RoomManager&) = delete;


    void addMessage(int roomId, const Message& msg);
    void addUser(int roomId, const User& user);

    std::vector<Message> getMessages(int roomId);
    std::vector<User> getUsers(int roomId);

    bool deleteMessage(int roomId, size_t index);

private:
    RoomManager();
    ~RoomManager();

    std::map<int, std::vector<Message>> mMessages;
    std::map<int, std::vector<User>> mUsers;

    std::mutex mMessagesMutex;
    std::mutex mUsersMutex;
};
