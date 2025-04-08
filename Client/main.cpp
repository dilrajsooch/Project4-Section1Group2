#include "raylib.h"
#include "string"
#include "iostream"
#include "Packet.h"
#include "PageSystem.h"
#include "RoomPage.h"
#include "LoginPage.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

using namespace std;

int main() {
    InitWindow(800, 450, "Bronny's Room");
    SetTargetFPS(60);

    // Create page system
    PageSystem* pageSystem = PageSystem::GetInstance();
    
    // Create pages
    LoginPage* loginPage = new LoginPage("Login");
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
