// src/view/InputFieldView.cpp
#include "view/InputFieldView.h"
#include "raylib.h"

void InputFieldView::draw(int x, int y, int width, int height, int fontSize, const std::string& text, bool focused) {
    // Draw the box
    DrawRectangle(x, y, width, height, focused ? LIGHTGRAY : GRAY);
    DrawRectangleLines(x, y, width, height, BLACK);

    // Draw the text
    DrawText(text.c_str(), x + 5, y + height / 4, fontSize, BLACK);
}
