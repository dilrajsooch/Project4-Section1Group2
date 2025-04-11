#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include "include/raygui.h"

#include "ServerState.h"
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
    int logScrollOffset = 0;

    void DrawServerInfo(const ServerState& state, const int& count, const std::vector<std::string>& logs);

    std::string ServerStateToString(ServerState st);
};
