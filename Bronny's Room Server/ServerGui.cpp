#include "include/ServerGui.h"
#include <string>
#include <vector>

void ServerGUI::Init(int screenWidth, int screenHeight) {
    InitWindow(screenWidth, screenHeight, "Bronney's Server");
    SetTargetFPS(60);
}

void ServerGUI::RunLoop() {
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        GlobalDataModel& model = GlobalDataModel::getInstance();
        ServerState state = model.GetState();
        int userCount = model.GetUserCount();
        std::vector<std::string> logs = model.GetLogs();

        DrawServerInfo(state, userCount, logs);

        EndDrawing();
    }
    CloseWindow();
}

void ServerGUI::DrawServerInfo(const ServerState& state, const int& count, const std::vector<std::string>& logs) {
    //Header Section
    DrawRectangle(0, 0, GetScreenWidth(), 40, LIGHTGRAY);
    std::string stateStr = ServerStateToString(state);
    std::string info = "State: " + stateStr + " | Connected Users: " + std::to_string(count);
    DrawText(info.c_str(), 10, 10, 20, BLACK);

    //Log Section
    int logPanelY = 40;
    DrawRectangle(0, logPanelY, GetScreenWidth(), GetScreenHeight() - logPanelY, RAYWHITE);

    int y = logPanelY + 5;
    for (const auto& log : logs) {
        // Optionally implement text wrapping or clipping for long log messages.
        DrawText(log.c_str(), 10, y, 16, DARKGRAY);
        y += 20;
        if (y > GetScreenHeight() - 20)
            break;
    }
}


std::string ServerGUI::ServerStateToString(ServerState st) {
    switch (st) {
    case ServerState::INITIALIZING:
        return "Initializing";
    case ServerState::RUNNING:
        return "Running";
    case ServerState::DEINITIALIZING:
        return "Deinitializing";
    case ServerState::TERMINATED:
        return "Terminated";
    default:
        return "Unknown";
    }
}
