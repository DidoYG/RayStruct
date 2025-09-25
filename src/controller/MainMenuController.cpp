// src/controller/MainMenuController.cpp
#include "controller/MainMenuController.h"
#include "raylib.h"

MainMenuController::MainMenuController()
: startButtonModel((GetScreenWidth() - 250) / 2, 500, 250, 60, 40, "Start"),
  exitButtonModel((GetScreenWidth() - 250) / 2, 700, 250, 60, 40, "Exit"),
  startButton(startButtonModel),
  exitButton(exitButtonModel) {}

void MainMenuController::update() {
    startButton.update();
    exitButton.update();
}

// draws the UI elements of the main menu
void MainMenuController::draw() {
    view.draw(); 
    startButton.draw();
    exitButton.draw();
}

bool MainMenuController::shouldStart() const {
    return startButton.wasClicked();
}

bool MainMenuController::shouldExit() const {
    return exitButton.wasClicked();
}
