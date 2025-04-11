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
    else {
        std::cout << "Opened database successfully" << std::endl;
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
