// include/model/InputFieldModel.h
#pragma once
#include <string>

class InputFieldModel {
private:
    int x, y, width, height, fontSize;
    std::string text = "";
    bool focused = false;

public:
    InputFieldModel(int x, int y, int w, int h, int fS);

    int getX() const;
    int getY() const;
    int getWidth() const;
    int getHeight() const;
    int getFontSize() const;
    const std::string& getText() const;
    bool isFocused() const;

    void setText(const std::string& newText);
    void setFocused(bool f);
};
