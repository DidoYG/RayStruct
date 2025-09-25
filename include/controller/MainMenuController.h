// include/controller/MainMenuController.h
#pragma once
#include "controller/ButtonController.h"
#include "view/MainMenuView.h"

class MainMenuController {
private:
    MainMenuView view;

    ButtonModel startButtonModel;
    ButtonModel exitButtonModel;

    ButtonController startButton;
    ButtonController exitButton;

public:
    MainMenuController();

    void update();
    void draw();

    bool shouldStart() const;
    bool shouldExit() const;
};
