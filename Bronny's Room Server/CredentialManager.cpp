#include "include/CredentialManager.h"
#include "include/SupabaseClient.h"

bool ValidateCredentials(const std::string& username,
    const std::string& password)
{
    std::string id;
    return SupabaseClient::Instance()
        .AuthenticateUser(username, password, id);
}

bool RegisterCredentials(const std::string& username,
    const std::string& password)
{
    std::string id;
    return SupabaseClient::Instance()
        .RegisterUser(username, password, id);
}

std::string GetAccountID(const std::string& username)
{
    std::string id;
    // Probe with impossible password so we only search by username
    SupabaseClient::Instance()
        .AuthenticateUser(username, "__dummy__", id);
    return id;
}