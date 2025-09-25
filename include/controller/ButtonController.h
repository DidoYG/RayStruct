// include/controller/ButtonController.h
#pragma once
#include "model/ButtonModel.h"
#include "view/ButtonView.h"

class ButtonController {
private:
    ButtonModel& model;
    ButtonView view;
public:
    ButtonController(ButtonModel& m);

    void update();
    void draw();
    bool wasClicked() const;
};
