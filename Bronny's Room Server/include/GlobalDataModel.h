#pragma once
#include <mutex>        
#include <string>       
#include <vector>       
#include <atomic>       
#include <iostream>     

#include "ServerState.h"

class GlobalDataModel {
public:
    GlobalDataModel(const GlobalDataModel&) = delete;
    GlobalDataModel& operator=(const GlobalDataModel&) = delete;
    GlobalDataModel(GlobalDataModel&&) = delete;
    GlobalDataModel& operator=(GlobalDataModel&&) = delete;

    static GlobalDataModel& getInstance() {
        static GlobalDataModel instance;
        return instance;
    }

    // ---------------------------
    // User Count Functions
    // ---------------------------
    void UserConnected();
    void UserDisconnected();
    int GetUserCount();

    // ---------------------------
    // Log Functions
    // ---------------------------
    void AddLog(const std::string& msg);
    std::vector<std::string> GetLogs();

    // ---------------------------
    // State Functions
    // ---------------------------
    void SetState(ServerState state);
    ServerState GetState();

private:
    // Private constructor for Singleton
    GlobalDataModel()
        : p_totalConnected(0)
        , p_state(ServerState::INITIALIZING) {
    }

    // Data members
    int p_totalConnected;
    std::mutex p_connectedMutex;

    std::vector<std::string> p_logs;
    std::mutex p_logMutex;

    ServerState p_state;
    std::mutex p_stateMutex;
};
