// include/controller/FileInputMenuController.h
#pragma once
#include "controller/ButtonController.h"
#include "controller/InputFieldController.h"
#include "view/FileInputMenuView.h"

class FileInputMenuController {
private:
    std::string title = "Enter File Path";

    FileInputMenuView view; 

    ButtonModel backButtonModel;
    ButtonModel validateButtonModel;
    ButtonModel uploadButtonModel;
    InputFieldModel inputFieldModel;

    ButtonController backButton;
    ButtonController validateButton;
    ButtonController uploadButton;
    InputFieldController inputField;

public:
    FileInputMenuController();

    void update();
    void draw();

    bool shouldGoBack();
    bool validateInput();
    std::string getInput() const;
    bool shouldValidate();
    bool uploadFile();
};