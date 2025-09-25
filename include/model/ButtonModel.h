// include/model/ButtonModel.h
#pragma once
#include <string>

class ButtonModel {
private:
    int x, y, width, height, fontSize;
    std::string label;
    bool hovered = false;
    bool clicked = false;
public:
    ButtonModel(int x, int y, int w, int h, int fS, const std::string& lb);

    void setLabel(const std::string& newLabel);

    int getX() const;
    int getY() const;
    int getWidth() const;
    int getHeight() const;
    int getFontSize() const;
    const std::string& getLabel() const;

    void setHovered(bool h);
    bool isHovered() const;

    void setClicked(bool c);
    bool isClicked() const;
};
