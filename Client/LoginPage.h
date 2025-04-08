#pragma once
#include "PageSystem.h"
#include "Page.h"
#include <string>
#include "CSocket.h"

class LoginPage : public Page {
private:

    // Connection Details
    const string IP = "127.0.0.1";
    const int PORT = 25565;

    // Error Text
    string error = "";

    // Login fields
    Rectangle loginPanel;
    Rectangle usernameBox;
    Rectangle passwordBox;
    Rectangle loginButton;
    Rectangle registerButton;
    Vector2 errorTextLabel;
    
    // Register fields
    Rectangle registerPanel;
    Rectangle regUsernameBox;
    Rectangle regPasswordBox;
    Rectangle regConfirmPasswordBox;
    Rectangle submitRegisterButton;
    Rectangle backToLoginButton;
    Vector2 regErrorTextLabel;
    
    // Input buffers
    char username[128] = "";
    char password[128] = "";
    char regUsername[128] = "";
    char regPassword[128] = "";
    char regConfirmPassword[128] = "";
    
    // State
    bool isRegistering = false;
    bool loginUsernameEditMode = false;
    bool loginPasswordEditMode = false;
    bool registerUsernameEditMode = false;
    bool registerPasswordEditMode = false;
    bool registerConfirmPasswordEditMode = false;

public:
    LoginPage(std::string name) : Page(name) {

        // Connect to server
        int err = CSocket::GetInstance()->Connect(IP, PORT);

        switch (err)
        {
            case -1:
                error = "Failed to connect to server!";
                break;
            case -2:
                error = "Ip is incorrect!";
                break;
        }

        // Initialize login panel
        loginPanel = { 200, 100, 400, 250 };
        usernameBox = { 250, 150, 300, 30 };
        passwordBox = { 250, 200, 300, 30 };
        loginButton = { 250, 250, 140, 30 };
        registerButton = { 410, 250, 140, 30 };
        errorTextLabel = { 210, 320 };
        
        // Initialize register panel
        registerPanel = { 200, 100, 400, 300 };
        regUsernameBox = { 250, 150, 300, 30 };
        regPasswordBox = { 250, 200, 300, 30 };
        regConfirmPasswordBox = { 250, 250, 300, 30 };
        submitRegisterButton = { 250, 310, 140, 30 };
        backToLoginButton = { 410, 310, 140, 30 };
        regErrorTextLabel = { 210, 370};
    }

    void Update() override {
        if (!isRegistering) {
            // Login panel logic
            if (GuiButton(loginButton, "Login")) {
                // Handle login
                std::string usernameStr(username);
                std::string passwordStr(password);
                // TODO: Implement login logic
                std::cout << "Login attempted with: " << usernameStr << std::endl;

                PageSystem::GetInstance()->SwitchToPage("Main Room");
            }

            if (GuiTextBox(usernameBox, username, 128, loginUsernameEditMode))
            {
                loginUsernameEditMode = !loginUsernameEditMode;
            }
            if (GuiTextBox(passwordBox, password, 128, loginPasswordEditMode))
            {
                loginPasswordEditMode = !loginPasswordEditMode;
            }
            
            
            if (GuiButton(registerButton, "Register")) {
                isRegistering = true;
            }
        } else {
            // Register panel logic
            if (GuiButton(submitRegisterButton, "Submit")) {
                // Handle registration
                std::string usernameStr(regUsername);
                std::string passwordStr(regPassword);
                std::string confirmPasswordStr(regConfirmPassword);
                
                if (passwordStr == confirmPasswordStr) {
                    // TODO: Implement registration logic
                    std::cout << "Registration attempted with: " << usernameStr << std::endl;
                } else {
                    // TODO: Show error message
                    std::cout << "Passwords do not match!" << std::endl;
                }
            }

            if (GuiTextBox(regUsernameBox, regUsername, 128, registerUsernameEditMode))
            {
                registerUsernameEditMode = !registerUsernameEditMode;
            }
            if (GuiTextBox(regPasswordBox, regPassword, 128, registerPasswordEditMode))
            {
                registerPasswordEditMode = !registerPasswordEditMode;
            }
            if (GuiTextBox(regConfirmPasswordBox, regConfirmPassword, 128, registerConfirmPasswordEditMode))
            {
                registerConfirmPasswordEditMode = !registerConfirmPasswordEditMode;
            }
            
            if (GuiButton(backToLoginButton, "Back to Login")) {
                isRegistering = false;
                // Clear registration fields
                memset(regUsername, 0, sizeof(regUsername));
                memset(regPassword, 0, sizeof(regPassword));
                memset(regConfirmPassword, 0, sizeof(regConfirmPassword));
            }
        }
    }

    void Draw() override {
        if (!isRegistering) {
            // Draw login panel
            GuiPanel(loginPanel, "Login");
            
            // Draw labels
            GuiLabel({ usernameBox.x, usernameBox.y - 20, 100, 20 }, "Username:");
            GuiLabel({ passwordBox.x, passwordBox.y - 20, 100, 20 }, "Password:");
            
            // Draw inputs
            GuiTextBox(usernameBox, username, 128, loginUsernameEditMode);
            GuiTextBox(passwordBox, password, 128, loginPasswordEditMode);
            
            // Draw buttons
            GuiButton(loginButton, "Login");
            GuiButton(registerButton, "Register");

            if (error != "")
            {
                DrawText(error.c_str(), errorTextLabel.x, errorTextLabel.y, 15, RED);
            }
        } else {
            // Draw register panel
            GuiPanel(registerPanel, "Register");
            
            // Draw labels
            GuiLabel({ regUsernameBox.x, regUsernameBox.y - 20, 100, 20 }, "Username:");
            GuiLabel({ regPasswordBox.x, regPasswordBox.y - 20, 100, 20 }, "Password:");
            GuiLabel({ regConfirmPasswordBox.x, regConfirmPasswordBox.y - 20, 150, 20 }, "Confirm Password:");
            
            // Draw inputs
            GuiTextBox(regUsernameBox, regUsername, 128, registerUsernameEditMode);
            GuiTextBox(regPasswordBox, regPassword, 128, registerPasswordEditMode);
            GuiTextBox(regConfirmPasswordBox, regConfirmPassword, 128, registerConfirmPasswordEditMode);
            
            // Draw buttons
            GuiButton(submitRegisterButton, "Submit");
            GuiButton(backToLoginButton, "Back to Login");
            if (error != "")
            {
                DrawText(error.c_str(), regErrorTextLabel.x, regErrorTextLabel.y, 15, RED);
            }
        }
    }
}; 