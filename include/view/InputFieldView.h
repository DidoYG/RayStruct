// include/view/InputFieldView.h
#pragma once
#include <string>

class InputFieldView {
public:
    void draw(int x, int y, int width, int height, int fontSize, const std::string& text, bool focused);
};
