#pragma once
#include "raygui.h"
#include "raylib.h"



int GuiCircleButton(Vector2 point, float radius, const char* text)
{
    int result = 0;
    int state = GuiGetState();
    int fontSize = 10;
   

    // Update control
    //--------------------------------------------------------------------
    if (state != STATE_DISABLED)
    {
        Vector2 mousePoint = GetMousePosition();

        // Check button state
        if (CheckCollisionPointCircle(mousePoint, point, radius)) 
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) state = STATE_PRESSED;
            else state = STATE_FOCUSED;

            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) result = 1;
        }
    }
    //--------------------------------------------------------------------

    // Draw control
    //--------------------------------------------------------------------
    Color borderColor = GetColor(GuiGetStyle(BUTTON, BORDER_COLOR_NORMAL + (state * 3)));
    Color backgroundColor = GetColor(GuiGetStyle(BUTTON, BASE_COLOR_NORMAL + (state * 3)));

    DrawCircleV(point, radius, backgroundColor);
    DrawCircleLinesV(point, radius, borderColor);

    Vector2 textSize = MeasureTextEx(GetFontDefault(), text, fontSize, 0);
    Vector2 textPos = { (point.x - (textSize.x / 2)), (point.y - (textSize.y / 2))  };

    DrawText(text, (int)textPos.x, (int)textPos.y, fontSize, GetColor(GuiGetStyle(BUTTON, TEXT_COLOR_NORMAL + (state * 3))));

    
    //------------------------------------------------------------------

    return result;      // Button pressed: result = 1
}

int GuiPost(Vector2 point, Post post)
{
    int result = 0;
    int state = GuiGetState();
    int fontSize = 10;

    const int padding = 10;
    const int profilePicSize = 32;
    const int spacing = 8;
    const int postWidth = 460;
    const int postHeight = 100;

    // Update control
    if (state != STATE_DISABLED)
    {
        Vector2 mousePoint = GetMousePosition();
        Rectangle bounds = { point.x, point.y, postWidth, postHeight }; // Fixed height or adjust dynamically

        if (CheckCollisionPointRec(mousePoint, bounds))
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) state = STATE_PRESSED;
            else state = STATE_FOCUSED;

            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) result = 1;
        }
    }

    // Draw background box
    Color borderColor = GetColor(GuiGetStyle(BUTTON, BORDER_COLOR_NORMAL + (state * 3)));
    Color backgroundColor = GetColor(GuiGetStyle(BUTTON, BASE_COLOR_NORMAL + (state * 3)));

    Rectangle postRect = { point.x, point.y, postWidth, postHeight }; // You can calculate height dynamically if needed
    DrawRectangleRec(postRect, backgroundColor);
    DrawRectangleLinesEx(postRect, 1, borderColor);

    // Draw profile picture
    User author = post.GetAuthor();
    Image profilePic = author.GetProfilePic();
    Texture2D profileTexture = LoadTextureFromImage(profilePic); // Consider caching this for performance

    Rectangle dest = { point.x + padding, point.y + padding, (float)profilePicSize, (float)profilePicSize };
    DrawTextureRec(profileTexture, { 0, 0, (float)profilePic.width, (float)profilePic.height }, { dest.x, dest.y }, WHITE);

    // Draw author name
    string authorName = author.GetUsername();
    DrawText(authorName.c_str(), (int)(dest.x + profilePicSize + spacing), (int)dest.y + 4, fontSize + 2, DARKGRAY);

    // Draw post text
    string postText = post.GetText();
    int textY = (int)(dest.y + profilePicSize + spacing);
    DrawText(postText.c_str(), (int)(point.x + padding), textY, fontSize, BLACK);

    // Draw image if available
    Image postImg = post.GetImage();
    if (postImg.width > 0 && postImg.height > 0)
    {
        Texture2D postTexture = LoadTextureFromImage(postImg);
        Rectangle imgDest = { point.x + padding, (float)(textY + 40), 200, 150 }; // Resize image
        DrawTextureRec(postTexture, { 0, 0, (float)postImg.width, (float)postImg.height }, { imgDest.x, imgDest.y }, WHITE);
    }

    return result;
}