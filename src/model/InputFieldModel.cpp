// src/model/InputFieldModel.cpp
#include "model/InputFieldModel.h"

InputFieldModel::InputFieldModel(int x, int y, int w, int h, int fS)
: x(x), y(y), width(w), height(h), fontSize(fS) {}

int InputFieldModel::getX() const { return x; }
int InputFieldModel::getY() const { return y; }
int InputFieldModel::getWidth() const { return width; }
int InputFieldModel::getHeight() const { return height; }
int InputFieldModel::getFontSize() const { return fontSize; }
const std::string& InputFieldModel::getText() const { return text; }
bool InputFieldModel::isFocused() const { return focused; }

void InputFieldModel::setText(const std::string& newText) { text = newText; }
void InputFieldModel::setFocused(bool f) { focused = f; }
