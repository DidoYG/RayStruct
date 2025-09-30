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

std::vector<int> PopulateMenuController::parseInput() const {
    std::vector<int> numbers;
    std::string input = inputField.getText();
    std::string currentNumber;

    if (visualizeButton.wasClicked()) {
        for (char ch : input) {
            if (isdigit(ch) || (ch == '-' && currentNumber.empty())) {
                currentNumber += ch; // build the current number
            } else if (ch == ' ' || ch == ',') {
                if (!currentNumber.empty()) {
                    numbers.push_back(std::stoi(currentNumber)); // convert to int and add to list
                    currentNumber.clear(); // reset for next number
                }
            }
        }

        // Add the last number if there's any
        if (!currentNumber.empty()) {
            numbers.push_back(std::stoi(currentNumber));
        }
    }

    return numbers;
}
