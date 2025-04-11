#include "include/CredentialManager.h"
#include "DBManager.h"
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
    auto db = DatabaseManager::getInstance()->getDB();
    if (!db) return false;

    const char* sql = "INSERT INTO Accounts (username, password) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) return false;

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    bool success = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);

    return success;
}

bool AuthenticateUser(const std::string& username, const std::string& password, std::string& accountId)
{
    auto db = DatabaseManager::getInstance()->getDB();
    if (!db) return false;

    const char* sql = "SELECT id FROM Accounts WHERE username = ? AND password = ?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) return false;

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    bool success = false;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0);
        accountId = std::to_string(id);
        success = true;
    }

    sqlite3_finalize(stmt);
    return success;
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
