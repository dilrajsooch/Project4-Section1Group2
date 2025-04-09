#include "include/CredentialManager.h"
#include <map>
#include <string>

struct UserInfo {
    std::string password;
    std::string accountID;
};

// Replace string-to-string map with string-to-UserInfo
static std::map<std::string, UserInfo> testUsers = {
    {"LeBron", {"Goat23", "1"}},
    {"Curry",  {"Chef30", "2"}},
    {"Kobe",   {"Mamba24", "3"}}
};

bool ValidateCredentials(const std::string& username, const std::string& password)
{
    auto it = testUsers.find(username);
    if (it != testUsers.end())
    {
        return (it->second.password == password);
    }
    return false;
}

bool RegisterCredentials(const std::string& username, const std::string& password)
{
    auto it = testUsers.find(username);
    if (it != testUsers.end())
    {
        return false;
    }

    std::string newID = std::to_string(testUsers.size() + 1);
    testUsers[username] = { password, newID };
    return true;
}

std::string GetAccountID(const std::string& username)
{
    auto it = testUsers.find(username);
    if (it != testUsers.end())
    {
        return it->second.accountID;
    }
    return ""; // Or throw or return a sentinel value
}
