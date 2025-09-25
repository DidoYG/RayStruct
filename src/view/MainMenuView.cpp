// src/view/MainMenuView.cpp
#include "view/MainMenuView.h"
#include "raylib.h"

void MainMenuView::draw() {
    std::string title = "Welcome to RayStruct++";
    int fontSize = 55;
    int textWidth = MeasureText(title.c_str(), fontSize);

    DrawText(title.c_str(),
             GetScreenWidth() / 2 - textWidth / 2,
             GetScreenHeight() / 4,
             fontSize,
             DARKBLUE);
}
