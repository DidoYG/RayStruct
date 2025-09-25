// src/controller/ChoiceMenuController.cpp
#include "controller/ChoiceMenuController.h"
#include "raylib.h"

ChoiceMenuController::ChoiceMenuController()
: label1("List"), 
  label2("Binary Tree"), 
  label3("Graph"),

  selectedStructure(DataStructure::NONE),
  
  backButtonModel(25, 25, 50, 50, 50, "<"),
  optionOneButtonModel((GetScreenWidth() - 315) * 1/4, 500, 315, 50, 30, label1),
  optionTwoButtonModel((GetScreenWidth() - 315) * 3/4, 500, 315, 50, 30, label2),
  optionThreeButtonModel((GetScreenWidth() - 315) * 1/4, 700, 315, 50, 30, label3),
  customButtonModel((GetScreenWidth() - 315) * 3/4, 700, 315, 50, 30, "Custom"),

  backButton(backButtonModel),
  optionOneButton(optionOneButtonModel),
  optionTwoButton(optionTwoButtonModel),
  optionThreeButton(optionThreeButtonModel),
  customButton(customButtonModel) {}

void ChoiceMenuController::setLabels(const std::string& l1, const std::string& l2, const std::string& l3) {
    optionOneButtonModel.setLabel(l1);
    optionTwoButtonModel.setLabel(l2);
    optionThreeButtonModel.setLabel(l3);
}

void ChoiceMenuController::update() {
    backButton.update();
    optionOneButton.update();
    optionTwoButton.update();
    optionThreeButton.update();
    customButton.update();
}

// draws the UI elements of the choice menu
void ChoiceMenuController::draw() {
    if (selectedStructure == DataStructure::NONE) {
        view.draw("Select Data Structure");
    }
    else {
        view.draw("Select Algorithm");
    }
    backButton.draw();
    optionOneButton.draw();
    optionTwoButton.draw();
    optionThreeButton.draw();
    customButton.draw();
}

bool ChoiceMenuController::shouldGoBack() const {
    return backButton.wasClicked();
}

DataStructure ChoiceMenuController::getSelectedStructure() {
    selectedStructure = DataStructure::NONE;
    if (optionOneButton.wasClicked()) selectedStructure = DataStructure::LIST;
    if (optionTwoButton.wasClicked()) selectedStructure = DataStructure::TREE;
    if (optionThreeButton.wasClicked()) selectedStructure = DataStructure::GRAPH;
    if (customButton.wasClicked()) selectedStructure = DataStructure::CUSTOM;
    return selectedStructure;
}

Algorithm ChoiceMenuController::getSelectedAlgorithm() const {
    if (selectedStructure == DataStructure::LIST) {
        if (optionOneButton.wasClicked()) return Algorithm::TRAVERSAL;
        if (optionTwoButton.wasClicked()) return Algorithm::INSERTION_SORT;
        if (optionThreeButton.wasClicked()) return Algorithm::MERGE_SORT;
    } else if(selectedStructure == DataStructure::TREE) {
        if (optionOneButton.wasClicked()) return Algorithm::BFS;
        if (optionTwoButton.wasClicked()) return Algorithm::INORDER;
        if (optionThreeButton.wasClicked()) return Algorithm::DELETION;
    } else if(selectedStructure == DataStructure::GRAPH) {
        if (optionOneButton.wasClicked()) return Algorithm::DIJKSTRA;
        if (optionTwoButton.wasClicked()) return Algorithm::PRIMS;
        if (optionThreeButton.wasClicked()) return Algorithm::TOPOLOGICAL_SORT;
    }
    return Algorithm::NONE;
}  