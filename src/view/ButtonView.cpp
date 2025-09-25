// src/view/ButtonView.cpp
#include "view/ButtonView.h"

void ButtonView::draw(int x, int y, int width, int height, int fontSize, const std::string& label, bool hovered) {
    Color color = hovered ? LIGHTGRAY : GRAY;
    DrawRectangle(x, y, width, height, color);

    int textWidth = MeasureText(label.c_str(), fontSize);
    DrawText(label.c_str(), x + (width - textWidth)/2, y + (height - fontSize)/2, fontSize, BLACK);
}
