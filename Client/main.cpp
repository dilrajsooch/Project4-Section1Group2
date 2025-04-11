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

#include <thread>


using namespace std;

// after installing a socket connnection, in the main page


void ListenForServerUpdates()
{
    while (true)
    {
        Packet packet = CSocket::GetInstance()->RecievePacket();
        switch (packet.GetType())
        {
            case ADD_POST:
            {
                int roomNumber = packet.Head.roomNumber;
                string postText(packet.GetText());
                Image img = packet.GetImage();

                Post newPost(roomNumber, postText, img, User("RemoteUser"));
                // add routing code to RoomPage or a ChatRoom manager
                break;
            }

            case ADD_ROOM:
            {
                string roomName(packet.GetText());
                ChatRoom newRoom(roomName);
                // TODO: Add RoomPage chatRooms 
                break;
            }

            default:
                break;
        }
    }
}

    //std::thread updateThread(ListenForServerUpdates);
    //updateThread.detach(); after successful login put in main



int main() {
    InitWindow(800, 450, "Bronny's Room");
    SetTargetFPS(60);
    

    // Create Socket
    CSocket* cSocket = CSocket::GetInstance();

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
