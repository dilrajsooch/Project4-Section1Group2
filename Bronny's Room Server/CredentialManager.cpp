#include "CredentialManager.h"
#include <map>
#include <string>

bool ValidateCredentials(const std::string& username, const std::string& password)
{
    // For local test data:
    static std::map<std::string, std::string> testUsers = {
        {"LeBron", "Goat23"},
        {"Curry", "Chef30"},
        {"Kobe", "Mamba24"}
    };

    auto it = testUsers.find(username);
    if (it != testUsers.end())
    {
        // If username found, compare password
        return (it->second == password);
    }
    return false;
}