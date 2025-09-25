// include/controller/PopulateMenuController.h
#pragma once
#include "controller/ButtonController.h"
#include "controller/InputFieldController.h"
#include "view/PopulateMenuView.h"

class PopulateMenuController {
private:
    PopulateMenuView view; 

    ButtonModel backButtonModel;
    ButtonModel visualizeButtonModel;
    InputFieldModel inputFieldModel;

    ButtonController backButton;
    ButtonController visualizeButton;
    InputFieldController inputField;

public:
    PopulateMenuController();

    void update();
    void draw();

    bool shouldGoBack();
    bool visualize() const;
};