// src/controller/VisualizationMenuController.cpp
#include "controller/VisualizationMenuController.h"
#include "raylib.h"

VisualizationMenuController::VisualizationMenuController()
: backButtonModel(25, 25, 50, 50, 50, "<"),
  backButton(backButtonModel) {}

void VisualizationMenuController::setNumbers(const std::vector<int>& nums) {
    numbers = nums;
} 

void VisualizationMenuController::setSelectedStructure(DataStructure ds) {
    selectedStructure = ds;
}

void VisualizationMenuController::setSelectedAlgorithm(Algorithm alg) {
    selectedAlgorithm = alg;
}

void VisualizationMenuController::update() {
    backButton.update();
}

// draws the UI elements of the choice menu
void VisualizationMenuController::draw() {
    view.draw(numbers); // temporary
    backButton.draw();
}

bool VisualizationMenuController::shouldGoBack() {
    return backButton.wasClicked();
}