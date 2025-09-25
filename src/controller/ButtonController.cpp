// src/controller/ButtonController.cpp
#include "controller/ButtonController.h"
#include "raylib.h"

ButtonController::ButtonController(ButtonModel& m) : model(m) {}

void ButtonController::update() {
    Vector2 mouse = GetMousePosition();
    bool hover = (mouse.x >= model.getX() &&
                  mouse.x <= model.getX() + model.getWidth() &&
                  mouse.y >= model.getY() &&
                  mouse.y <= model.getY() + model.getHeight());
    model.setHovered(hover);

    if (hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        model.setClicked(true);
    } else {
        model.setClicked(false);
    }
}

void ButtonController::draw() {
    view.draw(model.getX(), model.getY(), model.getWidth(), model.getHeight(),
              model.getFontSize(), model.getLabel(), model.isHovered());
}

bool ButtonController::wasClicked() const {
    return model.isClicked();
}
