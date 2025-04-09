#include "include/ServerGui.h"
#include <string>
#include <vector>

//-----------------------------------------------------------------------------
// Initializes the server window
//-----------------------------------------------------------------------------
void ServerGUI::Init(int screenWidth, int screenHeight) {
    // Initialize the window with a title.
    InitWindow(screenWidth, screenHeight, "Server GUI");
    // Set the frame rate for a smooth GUI experience.
    SetTargetFPS(60);
}

//-----------------------------------------------------------------------------
// Main loop: runs until the window should close
//-----------------------------------------------------------------------------
void ServerGUI::RunLoop() {
    while (!WindowShouldClose()) {  // Detect window close button or ESC key
        BeginDrawing();
        ClearBackground(RAYWHITE);  // Clear the screen with a white background

        // Get the current server state, user count, and log messages.
        GlobalDataModel& model = GlobalDataModel::getInstance();
        ServerState state = model.GetState();
        int userCount = model.GetUserCount();
        std::vector<std::string> logs = model.GetLogs();

        // Draw server information (state, user count, and logs) on the screen.
        DrawServerInfo(state, userCount, logs);

        EndDrawing();
    }
    // Close the window when the loop exits.
    CloseWindow();
}

//-----------------------------------------------------------------------------
// Draws server info including state, user count, and the last few log messages
//-----------------------------------------------------------------------------
void ServerGUI::DrawServerInfo(const ServerState& state, const int& count, const std::vector<std::string>& logs) {
    // Convert the server state to a string using our helper function.
    std::string stateStr = ServerStateToString(state);
    // Create a text string with state and the user count.
    std::string info = "State: " + stateStr + " | Connected Users: " + std::to_string(count);

    // Draw the info text at position (10,10) with font size 20 in black.
    DrawText(info.c_str(), 10, 10, 20, BLACK);

    // Draw log messages beginning at y = 40. We iterate through the logs,
    // drawing each and adjusting the y position so that they don't overlap.
    int y = 40;
    for (const auto& log : logs) {
        DrawText(log.c_str(), 10, y, 16, DARKGRAY);
        y += 20;
        // Prevent drawing off-screen by checking the current y position against the screen height.
        if (y > GetScreenHeight() - 20)
            break;
    }
}

//-----------------------------------------------------------------------------
// Converts a ServerState enum value to a readable string
//-----------------------------------------------------------------------------
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
