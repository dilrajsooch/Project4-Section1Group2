#pragma once
#include "include/GlobalDataModel.h"

// ---------------------------
// User Count Functions
// ---------------------------
void GlobalDataModel::UserConnected() {
    std::lock_guard<std::mutex> lock(p_connectedMutex);
    p_totalConnected++;
}
void GlobalDataModel::UserDisconnected() {
    std::lock_guard<std::mutex> lock(p_connectedMutex);
    p_totalConnected--;
}
int GlobalDataModel::GetUserCount() {
    std::lock_guard<std::mutex> lock(p_connectedMutex);
    return p_totalConnected;
}

// ---------------------------
// Log Functions
// ---------------------------
void GlobalDataModel::AddLog(const std::string& msg) {
    std::lock_guard<std::mutex> lock(p_logMutex);
    p_logs.push_back(msg);
}
std::vector<std::string> GlobalDataModel::GetLogs() {
    std::lock_guard<std::mutex> lock(p_logMutex);
    return p_logs;
}

// ---------------------------
// State Functions
// ---------------------------
void GlobalDataModel::SetState(ServerState state) {
    std::lock_guard<std::mutex> lock(p_stateMutex);
    p_state = state;
}
ServerState GlobalDataModel::GetState() {
    std::lock_guard<std::mutex> lock(p_stateMutex);
    return p_state;
}
