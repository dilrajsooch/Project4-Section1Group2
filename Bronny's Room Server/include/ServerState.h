#pragma once

enum class ServerState {
    INITIALIZING,
    RUNNING,
    DEINITIALIZING,
    TERMINATED
};

void SetServerState(ServerState newState);

ServerState GetServerState();
