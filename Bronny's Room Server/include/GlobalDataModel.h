#pragma once
#include <mutex>        
#include <string>       
#include <vector>       
#include <atomic>       
#include <iostream>     

#include "ServerState.h"

class GlobalDataModel {
public:
    GlobalDataModel()
        : p_totalConnected(0)
        , p_state(ServerState::INITIALIZING) { }

    //User Count functions
    void UserConnected() {
        std::lock_guard<std::mutex> lock(p_connectedMutex);
        p_totalConnected++;
    }
    void UserDisconnected() {
        std::lock_guard<std::mutex> lock(p_connectedMutex);
        p_totalConnected--;
    }
    int GetUserCount() { return p_totalConnected; }


    //Log functions
    void AddLog(const std::string& msg) {
        std::lock_guard<std::mutex> lock(p_logMutex);
        p_logs.push_back(msg);
    }
    std::vector<std::string> GetLogs() {
        std::lock_guard<std::mutex> lock(p_logMutex);
        return p_logs;
    }


    //State functions
    void SetState(ServerState state) {
        std::lock_guard<std::mutex> lock(p_stateMutex);
        this->p_state = state;
    }
    ServerState GetState() {
        std::lock_guard<std::mutex> lock(p_stateMutex);
        return p_state;
    }



private:
    int p_totalConnected;
    std::mutex p_connectedMutex;

    std::vector<std::string> p_logs;
    std::mutex p_logMutex;

    ServerState p_state;
    std::mutex p_stateMutex;

};