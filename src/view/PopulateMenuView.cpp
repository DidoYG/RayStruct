// src/view/PopulateMenuView.cpp
#include "view/PopulateMenuView.h"
#include "raylib.h"

void PopulateMenuView::draw() {
    std::string title = "Populate Data Structure";
    int fontSize = 40;
    int textWidth = MeasureText(title.c_str(), fontSize);

    DrawText(title.c_str(),
             GetScreenWidth() / 2 - textWidth / 2,
             GetScreenHeight() / 6,
             fontSize,
             DARKBLUE);
}
