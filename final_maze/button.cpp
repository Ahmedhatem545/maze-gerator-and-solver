#include "button.h"

button::button(const char* imagePath, Vector2 imagePosition, float scale) {
    // Load the image
    Image image = LoadImage(imagePath);
    int original_w = image.width;
    int original_h = image.height;

    // Calculate the new size based on the scale factor
    int newWidth = static_cast<int>(original_w * scale);
    int newHeight = static_cast<int>(original_h * scale);

    // Resize the image
    ImageResize(&image, newWidth, newHeight);

    // Load the texture from the resized image
    texture = LoadTextureFromImage(image);

    // Free the image data as it's no longer needed
    UnloadImage(image);

    // Set the position of the button
    positon = imagePosition;  

    isHovered = false;
}

button::~button() {
    // Unload the texture when the button is destroyed
    UnloadTexture(texture);
}

void button::Draw() {
    // Draw the button texture at its position
    DrawTextureV(texture, positon, isHovered ? GRAY : WHITE);
}

bool button::is_pressed(Vector2 mousePos, bool mousePressed) {
    // Create a rectangle for the button based on its position and size
    Rectangle rect = { positon.x, positon.y, static_cast<float>(texture.width), static_cast<float>(texture.height) };

    // Check for hover state
    isHovered = CheckCollisionPointRec(mousePos, rect); // Line 2 added

    if (isHovered && mousePressed) {
        return true;
    }
    return false;
}