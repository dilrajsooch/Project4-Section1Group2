#pragma once
#include "PageSystem.h"
#include "Packet.h"
#include "Page.h"
#include "CSocket.h"
#include "ChatRoom.h"
#include "vector"
#include "CustomElements.h"
#include <string>
#include <sstream>

class RoomPage : public Page {
private:
    Rectangle roomListPanelScrollView;
    Vector2 roomListPanelScrollOffset;
    float roomListPanelYOffset = 510;
    float roomChatPanelYOffset = 510;
    Rectangle roomChatPanelScrollView;
    Vector2 roomChatPanelScrollOffset;
    vector<ChatRoom> chatRooms;
    ChatRoom selectedRoom;
    char postText[256] = "";
    char createRoomText[128] = "";
    bool postTextEditMode = false;
    bool createRoomTextEditMode = false;
    int selectedDeletePost = -1;
    bool showCreateRoomPopup = false;
    bool hasGottenRooms = false;
    bool resetRooms = false;


public: 
    RoomPage(std::string name) : Page(name), selectedRoom("Empty", -1), roomListPanelScrollOffset({ 0,0 }), roomChatPanelScrollOffset({ 0,0 }) {
   
    }

    void GetRooms()
    {
        chatRooms.clear();

        // Use socket here.
        Packet pkt;
        pkt.SetType(Packet::GET_ROOMS);

        
        char* roomsString = CSocket::GetInstance()->SendPacket(pkt).Body.postText;

        istringstream roomStream(roomsString);
        string roomEntry;

        while (getline(roomStream, roomEntry, '|')) // split by '|'
        {
            istringstream entryStream(roomEntry);
            string roomID, roomName;

            if (getline(entryStream, roomID, ':') && getline(entryStream, roomName, ':'))
            {
                cout << "Room ID: " << roomID << ", Name: " << roomName << endl;
                ChatRoom newChatRoom(roomName, atoi(roomID.c_str()));
                chatRooms.push_back(newChatRoom);
            }
        }

    }


    void Update() override {
        if (!hasGottenRooms)
        {
            GetRooms();
            hasGottenRooms = true;
        }
        if (resetRooms)
        {
            GetRooms();
            selectedRoom.GetPosts();
            resetRooms = false;
        }
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
            {0, 0, 700, roomChatPanelYOffset},
            &roomChatPanelScrollOffset, 
            &roomChatPanelScrollView);
        DrawPosts();
        DrawText(selectedRoom.GetName().c_str(), 370, 35, 32, BLACK);
        if (GuiTextBox({ 100,400,600, 50 }, postText, 256, postTextEditMode))
        {
            postTextEditMode = !postTextEditMode;
        }

        if (GuiButton({700, 40, 32, 32}, GuiIconText(ICON_REPEAT_FILL, "")))
        {
            GetRooms();
            
            if (selectedRoom.GetRoomNumber() != -1)
            {
                selectedRoom.GetPosts();
            }
        }

        if (GuiButton({ 25, 400, 50, 50 }, "+"))
        {
            // Add Room Button
            showCreateRoomPopup = true;
        }

        if (showCreateRoomPopup)
        {
            if (showCreateRoomPopup)
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

                DrawRectangleRec(popupBounds, LIGHTGRAY);
                DrawRectangleLinesEx(popupBounds, 2, GRAY);

                const char* msg = "What is the name of the room you want to create?";
                int textWidth = MeasureText(msg, 10);
                DrawText(msg, (int)(popupBounds.x + (popupWidth - textWidth) / 2), (int)(popupBounds.y + 20), 10, BLACK);

                if (GuiTextBox({ popupBounds.x + (popupWidth / 2) - 75 ,popupBounds.y + 40, 150, 25}, createRoomText, 128, createRoomTextEditMode))
                {
                    createRoomTextEditMode = !createRoomTextEditMode;
                }

                Rectangle yesBtn = {
                    popupBounds.x + 40,
                    popupBounds.y + popupHeight - 40,
                    80, 25
                };
                Rectangle noBtn = {
                    popupBounds.x + popupWidth - 120,
                    popupBounds.y + popupHeight - 40,
                    80, 25
                };

                if (GuiButton(yesBtn, "Yes"))
                {
                    showCreateRoomPopup = false;
                    Packet pkt;
                    pkt.SetType(Packet::ADD_ROOM);
                    pkt.SetBody(createRoomText, strlen(createRoomText), false);

                    Packet result = CSocket::GetInstance()->SendPacket(pkt);
                    int code = atoi(result.GetText());
                    if (code < 0)
                    {
                        cout << "Failed to create room" << endl;
                    }
                    else
                    {
                        cout << "Room created" << endl;
                        GetRooms();
                    }
                    memset(createRoomText, 0, sizeof(createRoomText));
                }

                if (GuiButton(noBtn, "No"))
                {
                    showCreateRoomPopup = false;
                    memset(createRoomText, 0, sizeof(createRoomText));
                }
            }
        }

        if (GuiButton({ 700, 400, 100,50 }, "<<"))
        {
            if (strcmp(postText, "") == 0 || strcmp(selectedRoom.GetName().c_str(), "Empty") == 0) return;

            Packet pkt;
            pkt.SetRoomNumber(selectedRoom.GetRoomNumber());
            pkt.SetBody(postText, strlen(postText));
            pkt.SetType(Packet::ADD_POST);
            pkt.Head.userId = User::MainUser.GetId();

            CSocket::GetInstance()->SendPacket(pkt);

            memset(postText, 0, sizeof(postText));
            selectedRoom.GetPosts();
        }
    }

    void DrawPosts()
    {
        int postSpacing = 15;

        if (roomChatPanelYOffset + selectedRoom.GetPostCount() * 100 > roomChatPanelYOffset)
        {
            roomChatPanelYOffset = selectedRoom.GetPostCount() * (130);
        }

        for (int i = 0; i < selectedRoom.GetPostCount(); i++)
        {
            Post post = selectedRoom.GetPostByIndex(i);
            float y = roomChatPanelScrollView.y + i * ( 100 + postSpacing) + roomChatPanelScrollOffset.y;

            if (GuiPost({ roomChatPanelScrollView.x, y }, post, &selectedDeletePost, &resetRooms))
            {
                // reply logic here??
            }
        }
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
                selectedRoom.GetPosts();
            }

        }
    }

}; 