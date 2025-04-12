#include "raylib.h"
#include "string"
#include "iostream"
#include "Packet.h"
#include "PageSystem.h"
#include "RoomPage.h"
#include "LoginPage.h"
#include "CSocket.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

using namespace std;

int main(int argc, char* argv[]) 
{
    InitWindow(800, 450, "Bronny's Room");
    SetTargetFPS(60);

    string ip = "";
    
    if (argc >= 2)
    {
        ip = argv[1];
    }

    
    // Create Socket
    CSocket* cSocket = CSocket::GetInstance();

    // Create page system
    PageSystem* pageSystem = PageSystem::GetInstance();
    
    // Create pages
    LoginPage* loginPage = new LoginPage("Login", ip);
    RoomPage* roomPage = new RoomPage("Main Room");
    
    // Add pages to the system
    pageSystem->AddPage(loginPage);
    pageSystem->AddPage(roomPage);
    
    // Start with the login page
    pageSystem->SwitchToPage("Login");
    
    while (!WindowShouldClose())
    {
        // Update
        pageSystem->Update();
        
        // Draw
        BeginDrawing();
            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
            pageSystem->Draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
