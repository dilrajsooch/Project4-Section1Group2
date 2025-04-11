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

    // Check if username already exists
    const char* checkSql = "SELECT id FROM accounts WHERE username = ?;";
    sqlite3_stmt* checkStmt;
    if (sqlite3_prepare_v2(db, checkSql, -1, &checkStmt, nullptr) != SQLITE_OK) return false;
    
    sqlite3_bind_text(checkStmt, 1, username.c_str(), -1, SQLITE_STATIC);
    if (sqlite3_step(checkStmt) == SQLITE_ROW) {
        sqlite3_finalize(checkStmt);
        return false; // Username already exists
    }
    sqlite3_finalize(checkStmt);

    // Insert new user
    const char* sql = "INSERT INTO accounts (username, password) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) return false;

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    bool success = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);

    return success;
}

int AuthenticateUser(const std::string& username, const std::string& password)
{
    auto db = DatabaseManager::getInstance()->getDB();
    if (!db) return -1; // Database error

    const char* sql = "SELECT id FROM accounts WHERE username = ? AND password = ?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) return -1;

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    int userId = -1;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        userId = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return userId;
}

int RegisterUser(const std::string& username, const std::string& password)
{
    auto db = DatabaseManager::getInstance()->getDB();
    if (!db) return -1; // Database error

    // Check if username already exists
    const char* checkSql = "SELECT id FROM accounts WHERE username = ?;";
    sqlite3_stmt* checkStmt;
    if (sqlite3_prepare_v2(db, checkSql, -1, &checkStmt, nullptr) != SQLITE_OK) return -1;
    
    sqlite3_bind_text(checkStmt, 1, username.c_str(), -1, SQLITE_STATIC);
    if (sqlite3_step(checkStmt) == SQLITE_ROW) {
        sqlite3_finalize(checkStmt);
        return -2; // Username already exists
    }
    sqlite3_finalize(checkStmt);

    // Insert new user
    const char* sql = "INSERT INTO accounts (username, password) VALUES (?, ?);";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) return -1;

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return -1;
    }

    // Get the new user's ID
    int userId = sqlite3_last_insert_rowid(db);
    sqlite3_finalize(stmt);

    return userId;
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
