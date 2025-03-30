#pragma once
#include "PageSystem.h"
#include "Packet.h"
#include "Page.h"

class RoomPage : public Page {
private:
    Rectangle roomListPanelScrollView;
    Vector2 roomListPanelScrollOffset;
    Vector2 roomListPanelBoundsOffset;
    Rectangle roomChatPanelScrollView;
    Vector2 roomChatPanelScrollOffset;
    Vector2 roomChatPanelBoundsOffset;
    std::string roomTitle;


public:
    RoomPage(std::string name) : Page(name) {
        roomTitle = "Room Title goes here.";
    }


    void Update() override {
        // Add any update logic here
    }

    void Draw() override {
        // Draw your room interface
        GuiScrollPanel({ 0, 0, 104 - roomListPanelBoundsOffset.x, 728 - roomListPanelBoundsOffset.y }, 
                      NULL, { 0, 40, 104, 728 }, &roomListPanelScrollOffset, &roomListPanelScrollView);
        GuiScrollPanel({ 104, 96, 696 - roomChatPanelBoundsOffset.x, 672 - roomChatPanelBoundsOffset.y }, 
                      NULL, { 104, 96, 992, 672 }, &roomChatPanelScrollOffset, &roomChatPanelScrollView);
        GuiLabel({ 370, 35, 120, 24 }, roomTitle.c_str());
    }

    void SetRoomTitle(const std::string& title) {
        roomTitle = title;
    }

}; 