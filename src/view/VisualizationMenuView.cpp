// src/view/VisualizationMenuView.cpp
#include "view/VisualizationMenuView.h"
#include "raylib.h"

void VisualizationMenuView::draw(std::vector<int> numbers) { // temporary
    std::string title = "";

    for (int num : numbers) {
        title += std::to_string(num) + " ";
    }

    int fontSize = 40;
    int textWidth = MeasureText(title.c_str(), fontSize);

    DrawText(title.c_str(),
             GetScreenWidth() / 2 - textWidth / 2,
             GetScreenHeight() / 6,
             fontSize,
             DARKBLUE);
}
