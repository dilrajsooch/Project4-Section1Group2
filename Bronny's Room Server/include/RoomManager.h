#pragma once

#include <map>
#include <vector>
#include <mutex>
#include <string>

struct Post {
    bool IsImage;
    char* Data;
    int RoomId;
    int PostId;
    int UserId;
    int size;
};

struct User {
    int id;
    int socket;
};

struct Room {
    int id;
    std::string name;

    bool operator<(const Room& other) const {
        return id < other.id;
    }
};

class RoomManager {
public:
    static RoomManager& getInstance();

    RoomManager(const RoomManager&) = delete;
    RoomManager& operator=(const RoomManager&) = delete;


    int addMessage(int roomId, int userId, std::string msg);
    int addImage(int roomId, int userId, char* img, int size);
    void addUser(int roomId, const User& user);
    int addRoom(std::string);

    std::string getPosts(int roomId);
    std::vector<User> getUsers(int roomId);
    std::string getRooms();
    std::vector<Post> getImagePosts(int roomId);

    bool deletePost(int roomId, int messageId, int userId);

private:
    RoomManager();
    ~RoomManager();

    std::string SerializePosts(const std::vector<Post>& posts);

    std::map<Room, std::vector<Post>> mMessages;
    std::map<Room, std::vector<User>> mUsers;

    std::mutex mMessagesMutex;
    std::mutex mUsersMutex;
};
