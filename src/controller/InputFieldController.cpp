// src/controller/InputFieldController.cpp
#include "controller/InputFieldController.h"
#include "raylib.h"

InputFieldController::InputFieldController(InputFieldModel& m) : model(m) {}

void InputFieldController::update(bool isNumsOnly) {
    Vector2 mousePos = GetMousePosition();
    bool mouseOver = mousePos.x >= model.getX() && mousePos.x <= model.getX() + model.getWidth() &&
                     mousePos.y >= model.getY() && mousePos.y <= model.getY() + model.getHeight();

    // Focus if clicked
    if (mouseOver && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        model.setFocused(true);
    } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !mouseOver) {
        model.setFocused(false);
    }

    // Handle keyboard input if focused
    if (model.isFocused()) {
        int key = GetCharPressed();
        while (key > 0) {
            if (isNumsOnly) {
                if ((key >= '0' && key <= '9') || key == ',' || key == ' ') {
                    model.setText(model.getText() + static_cast<char>(key));
                }
            } else {
                model.setText(model.getText() + static_cast<char>(key));
            }
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE) && !model.getText().empty()) {
            std::string t = model.getText();
            t.pop_back();
            model.setText(t);
        }
    }
}

void InputFieldController::draw() {
    view.draw(model.getX(), model.getY(), model.getWidth(), model.getHeight(), model.getFontSize(),
              model.getText(), model.isFocused());
}

const std::string& InputFieldController::getText() const {
    return model.getText();
}
