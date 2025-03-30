#pragma once
#include "raylib.h"
#include "raygui.h"
#include <vector>
#include <string>
#include "Page.h"

class PageSystem {
private:
    std::vector<Page*> pages;
    int currentPageIndex;

public:
    PageSystem() : currentPageIndex(-1) {}
    ~PageSystem() {
        for (auto page : pages) {
            delete page;
        }
    }

    void AddPage(Page* page) {
        pages.push_back(page);
        if (currentPageIndex == -1) {
            currentPageIndex = 0;
            pages[0]->SetActive(true);
        }
    }

    void SwitchToPage(int index) {
        if (index >= 0 && index < pages.size()) {
            if (currentPageIndex != -1) {
                pages[currentPageIndex]->SetActive(false);
            }
            currentPageIndex = index;
            pages[currentPageIndex]->SetActive(true);
        }
    }

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

    void Update() {
        if (currentPageIndex != -1) {
            pages[currentPageIndex]->Update();
        }
    }

    void Draw() {
        if (currentPageIndex != -1) {
            pages[currentPageIndex]->Draw();
        }
    }

    int GetCurrentPageIndex() const { return currentPageIndex; }
}; 