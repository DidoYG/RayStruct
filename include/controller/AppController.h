// include/controller/AppController.h
#pragma once
#include "controller/MainMenuController.h"
#include "controller/ChoiceMenuController.h"
#include "controller/PopulateMenuController.h"

enum class AppState {
    MAIN_MENU,
    CHOICE_MENU,
    POPULATE_MENU,
    // VISUALIZATION_MENU,
    CUSTOM_MENU,
    EXIT
};

class AppController {
private:
    bool isAtAlgorithms;
    AppState state;
    DataStructure selectedStructure;
    Algorithm selectedAlgorithm;
    MainMenuController mainMenu;
    ChoiceMenuController choiceMenu;
    PopulateMenuController populateMenu;
    // FileInputController fileInputMenu; // for custom structures

public:
    AppController();

    void update();
    void draw();
    bool shouldClose() const;
};

