#include "DBManager.h"
#include <iostream>

DatabaseManager* DatabaseManager::instance = nullptr;

DatabaseManager::DatabaseManager() : db(nullptr) {}

DatabaseManager::~DatabaseManager() {
    if (db) {
        closeDatabase();
    }
}

DatabaseManager* DatabaseManager::getInstance() {
    if (!instance) {
        instance = new DatabaseManager();
    }
    return instance;
}

void DatabaseManager::destroyInstance() {
    if (instance) {
        delete instance;
        instance = nullptr;
    }
}

bool DatabaseManager::openDatabase(const std::string& dbName) {
    int rc = sqlite3_open(dbName.c_str(), &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        db = nullptr;
        return false;
    }

    // Enable foreign key constraints
    executeQuery("PRAGMA foreign_keys = ON;");

    std::cout << "Opened database and ensured tables exist." << std::endl;
    const char* schema = R"(
    CREATE TABLE IF NOT EXISTS accounts (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        username TEXT NOT NULL UNIQUE,
        password TEXT NOT NULL
    );

    CREATE TABLE IF NOT EXISTS login_log (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        account_id INTEGER NOT NULL,
        username TEXT NOT NULL,
        ip TEXT,
        timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
        FOREIGN KEY (account_id) REFERENCES accounts(id) ON DELETE CASCADE
    );
    )";

    if (!executeQuery(schema)) {
        std::cerr << "Failed to initialize database schema." << std::endl;
        return false;
    }
    return true;
}

void DatabaseManager::closeDatabase() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
        std::cout << "Database closed successfully" << std::endl;
    }
}

bool DatabaseManager::executeQuery(const std::string& query) {
    if (!db) {
        std::cerr << "Database not opened" << std::endl;
        return false;
    }

    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, query.c_str(), nullptr, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << (errMsg ? errMsg : "Unknown error") << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

sqlite3* DatabaseManager::getDB() {
    return db;
}
