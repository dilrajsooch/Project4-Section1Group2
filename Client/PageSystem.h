#pragma once
#include "raylib.h"
#include "raygui.h"
#include <vector>
#include <string>
#include "Page.h"

class PageSystem {
private:
    std::vector<Page*> pages; // All of the pages
    int currentPageIndex; // Current page indext

public:
    /// <summary>
    /// Defualt Constructor
    /// </summary>
    PageSystem() : currentPageIndex(-1) {}
    /// <summary>
    /// Default Deconstructor
    /// </summary>
    ~PageSystem() {
        for (auto page : pages) {
            delete page;
        }
    }
    
    /// <summary>
    /// Adds a page to the page system
    /// </summary>
    /// <param name="page">The page to be added</param>
    void AddPage(Page* page) {
        pages.push_back(page);
        if (currentPageIndex == -1) {
            currentPageIndex = 0;
            pages[0]->SetActive(true);
        }
    }

    /// <summary>
    /// Switches the active page of the system
    /// </summary>
    /// <param name="index">Index of the page to be switched to</param>
    void SwitchToPage(int index) {
        if (index >= 0 && index < pages.size()) {
            if (currentPageIndex != -1) {
                pages[currentPageIndex]->SetActive(false);
            }
            currentPageIndex = index;
            pages[currentPageIndex]->SetActive(true);
        }
    }

    /// <summary>
    /// Switches the active page of the system
    /// </summary>
    /// <param name="name">The name of the page to be switched to</param>
    void SwitchToPage(std::string name)
    {
        for (int i = 0; i < pages.size(); i++)
        {
            if (pages[i]->GetName() == name)
            {
                if (currentPageIndex != -1) {
                    pages[currentPageIndex]->SetActive(false);
                }
                currentPageIndex = i;
                pages[currentPageIndex]->SetActive(true);
            }
        }
    }

    /// <summary>
    /// Invokes the update of the currently active page
    /// </summary>
    void Update() {
        if (currentPageIndex != -1) {
            pages[currentPageIndex]->Update();
        }
    }

    /// <summary>
    /// Invokes the drawing of the currently active page
    /// </summary>
    void Draw() {
        if (currentPageIndex != -1) {
            pages[currentPageIndex]->Draw();
        }
    }

    /// <summary>
    /// Gets the current page indext
    /// </summary>
    /// <returns>The index of the currently active page</returns>
    int GetCurrentPageIndex() const { return currentPageIndex; }
}; 