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