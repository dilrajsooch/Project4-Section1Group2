#pragma once
#include "PageSystem.h"
#include "Packet.h"
#include "Page.h"
#include "ChatRoom.h"
#include "vector"
#include "CustomElements.h"

class RoomPage : public Page {
private:
    Rectangle roomListPanelScrollView;
    Vector2 roomListPanelScrollOffset;
    float roomListPanelYOffset = 510;
    Rectangle roomChatPanelScrollView;
    Vector2 roomChatPanelScrollOffset;
    vector<ChatRoom> chatRooms;
    ChatRoom selectedRoom;


public: 
    RoomPage(std::string name) : Page(name), selectedRoom("Empty"), roomListPanelScrollOffset({ 0,0 }), roomChatPanelScrollOffset({ 0,0 }) {
        GetRooms();
    }

    void GetRooms()
    {
        // Test code
        ChatRoom newChatroom("Test");
        chatRooms.push_back(newChatroom);

        newChatroom.SetName("Test2");

        chatRooms.push_back(newChatroom);

            newChatroom.SetName("Test2");
        
        // Use socket here.
    }


    void Update() override {
        // Add any update logic here
    }

    void Draw() override {
        // Draw your room interface
        
        GuiScrollPanel({ 0, 0, 100 , 470  }, 
            "Rooms", 
            { 0, 0, 100, roomListPanelYOffset }, 
            &roomListPanelScrollOffset, 
            &roomListPanelScrollView);
        DrawRoomButtons();
        GuiScrollPanel({ 100, 100, 700, 460}, 
            "ChatRoom", 
            {0, 0, 700, 672}, 
            &roomChatPanelScrollOffset, 
            &roomChatPanelScrollView);
        GuiLabel({ 370, 35, 120, 24 }, selectedRoom.GetName().c_str());
        
    }

    void DrawRoomButtons()
    {
        int buttonRadius = 32;
        int buttonSpacing = 5;

        if (roomListPanelYOffset + chatRooms.size() * buttonRadius * 2 > roomListPanelYOffset)
        {
            roomListPanelYOffset = chatRooms.size() * (buttonRadius * 2 + buttonSpacing);
        }

        for (int i = 0; i < chatRooms.size(); i++)
        {
           

            ChatRoom chatRoom = chatRooms.at(i);
            float y = roomListPanelScrollView.y + 40 + i * (buttonRadius * 2 + buttonSpacing) + roomListPanelScrollOffset.y;

            if (GuiCircleButton({ roomListPanelScrollView.x + 40, y }, buttonRadius, chatRoom.GetName().c_str()))
            {
                selectedRoom = chatRoom;
            }

        }
    }

}; 