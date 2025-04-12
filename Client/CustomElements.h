#pragma once
#include "raygui.h"
#include "raylib.h"
#include <string>


int GuiCircleButton(Vector2 point, float radius, const char* text)
{
    int result = 0;
    int state = GuiGetState();
    int fontSize = 10;

    if (state != STATE_DISABLED)
    {
        Vector2 mousePoint = GetMousePosition();

        if (CheckCollisionPointCircle(mousePoint, point, radius)) 
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) state = STATE_PRESSED;
            else state = STATE_FOCUSED;

            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) result = 1;
        }
    }

    Color borderColor = (state == STATE_PRESSED) ? Color{ 255, 215, 0, 255 } : BLACK;
    Color backgroundColor = (state == STATE_PRESSED) ? Color{ 255, 102, 0, 255 } : Color{ 210, 180, 140, 255 };

    DrawCircleV(point, radius, backgroundColor);
    DrawCircleLinesV(point, radius, borderColor);

    Vector2 textSize = MeasureTextEx(GetFontDefault(), text, fontSize, 0);
    Vector2 textPos = { (point.x - (textSize.x / 2)), (point.y - (textSize.y / 2)) };

    DrawText(text, (int)textPos.x, (int)textPos.y, fontSize, BLACK);

    return result;
}

int GuiPost(Vector2 point, Post post, int* selectedDeletePost, bool* resetRooms)
{
    int result = 0;
    int state = GuiGetState();
    int fontSize = 20;

    const int padding = 10;
    const int iconSize = 2;
    const int postWidth = 460;
    const int postHeight = 100;

    Color borderColor = Color{ 255, 102, 0, 255 };
    Color backgroundColor = Color{ 255, 239, 213, 255 };

    Rectangle postRect = { point.x, point.y, postWidth, postHeight };
    DrawRectangleRec(postRect, backgroundColor);
    DrawRectangleLinesEx(postRect, 2, borderColor);

    std::string postText = post.GetText();
    DrawText(postText.c_str(), (int)(point.x + padding), point.y + 40, fontSize, BLACK);

    Image postImg = post.GetImage();
    if (postImg.width > 0 && postImg.height > 0)
    {
        Texture2D postTexture = LoadTextureFromImage(postImg);
        Rectangle imgDest = { point.x + padding, point.y + 40, 200, 150 };
        DrawTextureRec(postTexture, { 0, 0, (float)postImg.width, (float)postImg.height }, { imgDest.x, imgDest.y }, WHITE);
    }

    Rectangle trashBounds = {
        point.x + postWidth - padding - iconSize * 10,
        point.y + padding + 20,
        (float)iconSize * 12,
        (float)iconSize * 14
    };

    if (post.GetUserID() == User::MainUser.GetId())
    {
        GuiDrawIcon(ICON_BIN, (int)trashBounds.x, (int)trashBounds.y, iconSize, RED);
    }

    if (CheckCollisionPointRec(GetMousePosition(), trashBounds))
    {
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            state = STATE_PRESSED;
        }
        else
        {
            state = STATE_FOCUSED;
        }

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) 
        {
            *selectedDeletePost = post.GetID();
        }
    }

    if (post.GetID() == *selectedDeletePost)
    {
        int popupWidth = 280;
        int popupHeight = 120;
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();

        Rectangle popupBounds = {
            (screenWidth - popupWidth) / 2.0f,
            (screenHeight - popupHeight) / 2.0f,
            (float)popupWidth,
            (float)popupHeight
        };

        DrawRectangleRec(popupBounds, Color{ 255, 248, 220, 255 });
        DrawRectangleLinesEx(popupBounds, 2, Color{ 255, 140, 0, 255 });

        const char* msg = "Are you sure you want to delete this post?";
        int textWidth = MeasureText(msg, 10);
        DrawText(msg, (int)(popupBounds.x + (popupWidth - textWidth) / 2), (int)(popupBounds.y + 20), 10, BLACK);

        Rectangle yesBtn = { popupBounds.x + 40, popupBounds.y + popupHeight - 40, 80, 25 };
        Rectangle noBtn = { popupBounds.x + popupWidth - 120, popupBounds.y + popupHeight - 40, 80, 25 };

        if (GuiButton(yesBtn, "Yes"))
        {
            *selectedDeletePost = -1;
            Packet pkt;
            pkt.SetUserId(User::MainUser.GetId());
            pkt.SetRoomNumber(post.GetRoomNumber());
            std::string postId = std::to_string(post.GetID());
            pkt.SetBody(postId.c_str(), strlen(postId.c_str()));
            pkt.SetType(Packet::DELETE_POST);
            CSocket::GetInstance()->SendPacket(pkt);
            *resetRooms = true;
        }

        if (GuiButton(noBtn, "No"))
        {
            *selectedDeletePost = -1;
        }
    }

    return result;
}



