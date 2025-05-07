#pragma once
#include <raylib.h>

class button
{
public:
    button(const char* imagePath, Vector2 position, float scale);
    ~button();
    void Draw();
    bool is_pressed(Vector2 mousePos, bool mousePressed);

private:
    Texture2D texture;
    Vector2 positon; // Typo kept as requested
    bool isHovered;
};