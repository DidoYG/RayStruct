// include/controller/AppController.h
#pragma once
#include "controller/MainMenuController.h"
#include "controller/ChoiceMenuController.h"
#include "controller/PopulateMenuController.h"
#include "controller/VisualizationMenuController.h"
#include "controller/FileInputMenuController.h"

enum class AppState {
    MAIN_MENU,
    CHOICE_MENU,
    POPULATE_MENU,
    VISUALIZATION_MENU,
    FILE_INPUT_MENU,
    EXIT
};

class AppController {
private:
    bool isAtAlgorithms;
    bool isValidPath;
    std::string path;
    std::vector<int> numbers; // store the numbers to be visualized
    AppState state;
    DataStructure selectedStructure;
    Algorithm selectedAlgorithm;
    MainMenuController mainMenu;
    ChoiceMenuController choiceMenu;
    PopulateMenuController populateMenu;
    VisualizationMenuController visualizationMenu;
    FileInputMenuController fileInputMenu;

public:
    AppController();

    void update();
    void draw();
    bool shouldClose() const;
};

