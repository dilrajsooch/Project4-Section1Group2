#pragma once
#include "PageSystem.h"
#include "Packet.h"
#include "Page.h"
#include "CSocket.h"
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
    char postText[256] = "";
    bool postTextEditMode = false;
    bool showPopup = false;


public: 
    RoomPage(std::string name) : Page(name), selectedRoom("Empty", -1), roomListPanelScrollOffset({ 0,0 }), roomChatPanelScrollOffset({ 0,0 }) {
   
    }

    void GetRooms()
    {
        // Test code
        ChatRoom newChatroom("Test", 0);
        chatRooms.push_back(newChatroom);

        newChatroom.SetName("Test2");
        newChatroom.SetRoomNumber(1);
        User::MainUser.SetId(2);

        User newUser(3);

        Post post(2, "Hello everyone!", User::MainUser.GetId());
       

        newChatroom.AddPost(post);

        Post post2(3, "Hello everyone!", newUser.GetId());

        newChatroom.AddPost(post2);

        chatRooms.push_back(newChatroom);
        
        // Use socket here.
        Packet pkt;
        pkt.SetType(Packet::GET_ROOMS);

        char* roomsString = CSocket::GetInstance()->SendPacket(pkt).Body.postText;

        char* roomsSplit = strtok(roomsString, "|");

        while (roomsSplit != NULL && strcmp(roomsSplit, "-1") != 0)
        {
            cout << roomsSplit << endl;
        }

    }


    void Update() override {
        if (chatRooms.size() == 0)
        {
            GetRooms();
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
            {0, 0, 700, 672}, 
            &roomChatPanelScrollOffset, 
            &roomChatPanelScrollView);
        DrawPosts();
        GuiLabel({ 370, 35, 120, 24 }, selectedRoom.GetName().c_str());
        if (GuiTextBox({ 100,400,600, 50 }, postText, 256, postTextEditMode))
        {
            postTextEditMode = !postTextEditMode;
        }

        if (GuiButton({ 700, 400, 100,50 }, "<<"))
        {
            if (strcmp(postText, "") == 0) return;

            Packet pkt;
            pkt.SetRoomNumber(selectedRoom.GetRoomNumber());
            pkt.SetBody(postText, strlen(postText));
            pkt.SetType(Packet::ADD_POST);

            CSocket::GetInstance()->SendPacket(pkt);
        }
    }

    void DrawPosts()
    {
        int postSpacing = 15;
        for (int i = 0; i < selectedRoom.GetPostCount(); i++)
        {
            Post post = selectedRoom.GetPostByIndex(i);
            float y = roomChatPanelScrollView.y + i * ( 100 + postSpacing) + roomChatPanelScrollOffset.y;

            if (GuiPost({ roomChatPanelScrollView.x, y }, post, &showPopup))
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
            }

        }
    }

}; 