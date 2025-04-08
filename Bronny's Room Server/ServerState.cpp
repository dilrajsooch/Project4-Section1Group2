#include "include/ServerState.h"
#include "include/GlobalDataModel.h"
#include <mutex>

static ServerState g_serverState = ServerState::INITIALIZING;
static std::mutex g_stateMutex;  // to protect state changes

void SetServerState(ServerState newState) {
    GlobalDataModel::getInstance().SetState(newState);
    std::lock_guard<std::mutex> lock(g_stateMutex);
    g_serverState = newState;
}

ServerState GetServerState() {
    std::lock_guard<std::mutex> lock(g_stateMutex);
    return g_serverState;
}
