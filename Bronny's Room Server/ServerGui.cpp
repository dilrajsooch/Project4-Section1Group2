#include "ServerGui.h"
#include <string>
#include <vector>

void ServerGUI::Init(int screenWidth, int screenHeight) {
    InitWindow(screenWidth, screenHeight, "Bronney's Server");
    SetTargetFPS(60);
    // Load a basketball icon
    Image icon = LoadImage("resources/basketball_icon.png");
    SetWindowIcon(icon);
    UnloadImage(icon);
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
    //Header
    std::string stateStr = ServerStateToString(state);
    std::string info = "State: " + stateStr + " | Connected Users: " + std::to_string(count);
    DrawText(info.c_str(), 10, 10, 20, ORANGE);

    const int logPanelY = 40;

    //Check for scrolling with mouse wheel
    logScrollOffset -= static_cast<int>(GetMouseWheelMove() * 20);
    //Get the total hight of the log list
    const int totalLogHeight = static_cast<int>(logs.size()) * 20;

    //Reset log offset if not enough logs
    if (logScrollOffset < 0)
        logScrollOffset = 0;

    //Calculate the scroll offset
    int visibleArea = GetScreenHeight() - logPanelY;
    if (totalLogHeight > visibleArea) {
        int maxOffset = totalLogHeight - visibleArea;
        if (logScrollOffset > maxOffset)
            logScrollOffset = maxOffset;
    }
    else {
        logScrollOffset = 0;
    }

    // Draw background with basketball theme color
    DrawRectangle(0, logPanelY, GetScreenWidth(), GetScreenHeight() - logPanelY, ORANGE);

    //Draw logs using the scroll offset
    int y = logPanelY - logScrollOffset;
    for (const auto& log : logs) {
        //Only draw if is visible
        if (y >= logPanelY && y < GetScreenHeight() - 20) {
            DrawText(log.c_str(), 10, y, 16, BLACK);
        }
        y += 20;
    }

    //Draw scroll bar
    if (totalLogHeight > visibleArea) {
        //Calculate scrollbar height vs visible area
        int scrollBarHeight = visibleArea * visibleArea / totalLogHeight;
        //Calculate the scrollbar position relative to the scroll offset
        int scrollBarY = logPanelY + (logScrollOffset * visibleArea / totalLogHeight);
        DrawRectangle(GetScreenWidth() - 15, scrollBarY, 10, scrollBarHeight, BLACK);
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
