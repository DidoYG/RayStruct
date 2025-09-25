// include/controller/InputFieldController.h
#pragma once
#include "model/InputFieldModel.h"
#include "view/InputFieldView.h"

class InputFieldController {
private:
    InputFieldModel& model;
    InputFieldView view;

public:
    InputFieldController(InputFieldModel& m);

    void update(bool isNumsOnly);  // Handle user typing, focus, etc.
    void draw();    // Draw using the view

    const std::string& getText() const;  // Expose text to outside
};
