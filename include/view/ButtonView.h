// include/view/ButtonView.h
#pragma once
#include <string>
#include "raylib.h"

class ButtonView {
public:
    void draw(int x, int y, int width, int height, int fontSize, const std::string& label, bool hovered);
};
