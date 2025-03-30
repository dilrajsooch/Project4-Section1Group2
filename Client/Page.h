#pragma once
class Page {
protected:
    bool isActive;
    std::string name;
public:
    Page(std::string name) : name(name), isActive(false) {}
    virtual ~Page() = default;

    virtual void Update() = 0;
    virtual void Draw() = 0;

    void SetActive(bool active) { isActive = active; }
    bool IsActive() const { return isActive; }

    std::string GetName() { return name; }

};