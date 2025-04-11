#include "include/CredentialManager.h"
#include "DBManager.h"
#include <string>

bool ValidateCredentials(const std::string& username, const std::string& password)
{
    auto db = DatabaseManager::getInstance()->getDB();
    if (!db) return false;

    const char* sql = "SELECT id FROM accounts WHERE username = ? AND password = ?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) return false;

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    bool success = sqlite3_step(stmt) == SQLITE_ROW;
    sqlite3_finalize(stmt);

    return success;
}

bool RegisterCredentials(const std::string& username, const std::string& password)
{
    auto db = DatabaseManager::getInstance()->getDB();
    if (!db) return false;

    const char* sql = "INSERT INTO accounts (username, password) VALUES (?, ?);";
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

    const char* sql = "SELECT id FROM accounts WHERE username = ? AND password = ?;";
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
    auto db = DatabaseManager::getInstance()->getDB();
    if (!db) return "";

    const char* sql = "SELECT id FROM accounts WHERE username = ?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) return "";

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    std::string accountId;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0);
        accountId = std::to_string(id);
    }

    sqlite3_finalize(stmt);
    return accountId;
}
