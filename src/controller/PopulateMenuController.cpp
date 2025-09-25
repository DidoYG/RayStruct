// src/controller/PopulateMenuController.cpp
#include "controller/PopulateMenuController.h"
#include "raylib.h"

PopulateMenuController::PopulateMenuController()
: backButtonModel(25, 25, 50, 50, 50, "<"),
  visualizeButtonModel((GetScreenWidth() - 250) / 2, 800, 250, 50, 30, "Visualize"),
  inputFieldModel((GetScreenWidth() - 500) / 2, 400, 500, 50, 30),

  backButton(backButtonModel),
  visualizeButton(visualizeButtonModel),
  inputField(inputFieldModel) {}

void PopulateMenuController::update() {
    backButton.update();
    visualizeButton.update();
    inputField.update(true); // only allow numbers, spaces, and commas
}

// draws the UI elements of the choice menu
void PopulateMenuController::draw() {
    view.draw(); 
    backButton.draw();
    visualizeButton.draw();
    inputField.draw();
}

bool PopulateMenuController::shouldGoBack() {
    if (backButton.wasClicked()) {
        inputFieldModel.setText(""); // clear input field when going back
        return true;
    } else {
        return false;
    }
}

bool PopulateMenuController::visualize() const {
    return visualizeButton.wasClicked(); // make it return the list of numbers to the app controller
}

// sanitize the input to convert it into a list of numbers
// make the call when the visualize button is clicked
// make a separate menu for file path input with a validate path button and dynamic text showing if the path is valid or not
// start git repo and push changes