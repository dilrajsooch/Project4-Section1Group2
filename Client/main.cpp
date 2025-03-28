#include "raylib.h"
#include "string"
#include "iostream"
#include "Packet.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

using namespace std;

int main(int argc, char *argv[]) {
    InitWindow(800, 450, "Bronny's Room");
    SetTargetFPS(60);

    // Declare variables to be used with elements
    Rectangle roomListPanelScrollView = { 0, 0, 0, 0 };
    Vector2 roomListPanelScrollOffset = { 0, 0 };
    Vector2 roomListPanelBoundsOffset = { 0, 0 };
    Rectangle roomChatPanelScrollView = { 0, 0, 0, 0 };
    Vector2 roomChatPanelScrollOffset = { 0, 0 };
    Vector2 roomChatPanelBoundsOffset = { 0, 0 };
    string roomTitle = "Room Title goes here.";

    //------------------------ Testing Packet and Image stuff
    Image image = LoadImage("C:/Users/jaxdr/OneDrive - Conestoga College/School/BCS/Y2S2 ---/Project 4/Project4-Section1Group2/Client/x64/Debug/test.png");
    int numChannels = image.format == 3; // RGBA: 4 channels, RGB: 3 channels

    // Calculate the size of the image in bytes
    int imageSizeInBytes = image.width * image.height * numChannels;

    // Log the image size in bytes (for debugging)
    printf("Image Size: %d bytes\n", imageSizeInBytes);

    Packet newPacket;

    newPacket.SetBody("Tester", 7);

    cout << newPacket.GetText() << endl;

    Packet testPacket(newPacket.SerializeData());

    cout << testPacket.GetText() << endl;

    ImageResize(&image, 256, 256);
    Texture2D texture = LoadTextureFromImage(image);

    UnloadImage(image);
    // ------------------------------- End of Testing Stuff
    
    while (!WindowShouldClose())
    {
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

            DrawTextureRec(texture, {0, 0, 256, 256}, {0, 0}, WHITE);
            /*GuiScrollPanel({ 0, 0, 104 - roomListPanelBoundsOffset.x, 728 - roomListPanelBoundsOffset.y }, NULL, { 0, 40, 104, 728 }, &roomListPanelScrollOffset, &roomListPanelScrollView);
            GuiScrollPanel({ 104, 96, 696 - roomChatPanelBoundsOffset.x, 672 - roomChatPanelBoundsOffset.y }, NULL,  { 104, 96, 992, 672 }, &roomChatPanelScrollOffset, &roomChatPanelScrollView);
            GuiLabel({ 370, 35, 120, 24 }, roomTitle.c_str());*/


        EndDrawing();
    }

    UnloadTexture(texture);

    CloseWindow();

    return 0;
}
