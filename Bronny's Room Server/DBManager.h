#pragma once

#include <sqlite3.h>
#include <string>

class DatabaseManager {
public:
    static DatabaseManager* getInstance();

    static void destroyInstance();

    bool openDatabase(const std::string& dbName);

    void closeDatabase();
    
    bool executeQuery(const std::string& query);

    sqlite3* getDB();

private:
    DatabaseManager();

    ~DatabaseManager();

    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    static DatabaseManager* instance;

    sqlite3* db;
};
