// src/model/ButtonModel.cpp
#include "model/ButtonModel.h"

ButtonModel::ButtonModel(int x, int y, int w, int h, int fS, const std::string& lb): x(x), y(y), width(w), height(h), fontSize(fS), label(lb) {}

void ButtonModel::setLabel(const std::string& newLabel) { label = newLabel; }

int ButtonModel::getX() const { return x; }
int ButtonModel::getY() const { return y; }
int ButtonModel::getWidth() const { return width; }
int ButtonModel::getHeight() const { return height; }
int ButtonModel::getFontSize() const { return fontSize; }
const std::string& ButtonModel::getLabel() const { return label; }

void ButtonModel::setHovered(bool h) { hovered = h; }
bool ButtonModel::isHovered() const { return hovered; }

void ButtonModel::setClicked(bool c) { clicked = c; }
bool ButtonModel::isClicked() const { return clicked; }
