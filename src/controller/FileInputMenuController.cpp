// src/controller/FileInputMenuController.cpp
#include "controller/FileInputMenuController.h"
#include "raylib.h"
#include <filesystem>

namespace fs = std::filesystem;

FileInputMenuController::FileInputMenuController()
: backButtonModel(25, 25, 50, 50, 50, "<"),
  validateButtonModel((GetScreenWidth() - 250) * 1/4, 800, 250, 50, 30, "Validate Path"),
  uploadButtonModel((GetScreenWidth() - 250) * 3/4, 800, 250, 50, 30, "Upload File"),
  inputFieldModel((GetScreenWidth() - 1000) / 2, 400, 1000, 50, 30),

  backButton(backButtonModel),
  validateButton(validateButtonModel),
  uploadButton(uploadButtonModel),
  inputField(inputFieldModel) {}

void FileInputMenuController::update() {
    backButton.update();
    validateButton.update();
    uploadButton.update();
    inputField.update(false); // only allow numbers, spaces, and commas
}

// draws the UI elements of the choice menu
void FileInputMenuController::draw() {
    view.draw(title); 
    backButton.draw();
    validateButton.draw();
    uploadButton.draw();
    inputField.draw();
}

bool FileInputMenuController::shouldGoBack() {
    if (backButton.wasClicked()) {
        title = "Enter File Path";
        inputFieldModel.setText(""); // clear input field when going back
        return true;
    } else {
        return false;
    }
}

bool FileInputMenuController::validateInput() {
    std::string path = inputFieldModel.getText();  

    // trim leading/trailing spaces (optional)
    path.erase(0, path.find_first_not_of(" \t\n\r"));
    path.erase(path.find_last_not_of(" \t\n\r") + 1);

    if (!path.empty() && fs::exists(path)) {
        title = "Valid Path!";
        return true;  // path exists
    } else {
        title = "Invalid Path!";
        return false; // invalid or missing
    }
}

std::string FileInputMenuController::getInput() const {
    return inputField.getText();
}

bool FileInputMenuController::shouldValidate() {
    return validateButton.wasClicked();
}

bool FileInputMenuController::uploadFile() {
    // title = "Enter File Path";
    // inputFieldModel.setText("");
    return uploadButton.wasClicked();
}