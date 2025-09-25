// src/view/ChoiceMenuController.cpp
#include "view/ChoiceMenuView.h"
#include "raylib.h"

void ChoiceMenuView::draw(const std::string& title) {
    int fontSize = 40;
    int textWidth = MeasureText(title.c_str(), fontSize);

    DrawText(title.c_str(),
             GetScreenWidth() / 2 - textWidth / 2,
             GetScreenHeight() / 6,
             fontSize,
             DARKBLUE);
}
