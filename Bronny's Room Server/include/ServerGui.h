#pragma once
#include "raylib.h"
#include "raygui.h"
#include "GlobalDataModel.h"

class ServerGUI
{
public:
    ServerGUI() { }

    ~ServerGUI() { }

    void Init(int screenWidth, int screenHeight);

    //Blocking
    void RunLoop();

private:
    ServerModel& serverModel_;

    void DrawServerInfo(const ServerState& state, cont int& count, const std::vector<std::string>& logs);

    std::string ServerStateToString(ServerState st);
};
