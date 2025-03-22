#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"


int main(int argc, char *argv[]) {
    InitWindow(800, 450, "Bronny's Room");
    SetTargetFPS(60);

    // Declare variables to be used with elements
    bool showMessageBox = false;
    bool inputFieldEditMode = false;
    char inputFieldText[128] = "Enter Command";
    bool filterTypeDropdownEditMode = false;
    int filterTypeDropdownActive = 0;
    bool sendButtonPressed = false;
    bool exitButtonPressed = false;

    while (!WindowShouldClose())
    {
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));


            if (filterTypeDropdownEditMode) GuiLock();

            GuiLabel({ 350, 80, 160, 24 }, "Disconnected");
            if (GuiTextBox( { 350, 128, 120, 24 }, inputFieldText, 128, inputFieldEditMode)) inputFieldEditMode = !inputFieldEditMode;
            sendButtonPressed = GuiButton( { 350, 232, 120, 24 }, "Send");
            exitButtonPressed = GuiButton( { 350, 288, 120, 24 }, "Exit");
            if (GuiDropdownBox( { 350, 184, 136, 24 }, "Select Filter Type;AUTHOR;TOPIC", & filterTypeDropdownActive, filterTypeDropdownEditMode)) filterTypeDropdownEditMode = !filterTypeDropdownEditMode;

            GuiUnlock();


            // Button with response example
            if (GuiButton({ 24, 24, 120, 30 }, "#191#Show Message"))
            {
                showMessageBox = true;
            }

            if (showMessageBox)
            {
                int result = GuiMessageBox({ 85, 70, 250, 100 },
                    "#191#Message Box", "Hi! This is a message!", "Nice;Cool");

                if (result >= 0)
                {
                    showMessageBox = false;
                }
            }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
