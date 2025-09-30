// src/controller/AppController.cpp
#include "controller/AppController.h"
#include "raylib.h"

AppController::AppController()
: isAtAlgorithms(false), 
  isValidPath(false),
  state(AppState::MAIN_MENU),
  selectedStructure(DataStructure::NONE),
  selectedAlgorithm(Algorithm::NONE) {}

void AppController::update() {
    switch (state) {
        case AppState::MAIN_MENU:
            mainMenu.update();
            if (mainMenu.shouldStart()) {
                state = AppState::CHOICE_MENU;
            }

            if (mainMenu.shouldExit()) {
                state = AppState::EXIT;
            }

            break;

        case AppState::CHOICE_MENU:
            choiceMenu.update();
            if (choiceMenu.shouldGoBack()) {                
                if (isAtAlgorithms) {
                    choiceMenu.setLabels("List", "Binary Tree", "Graph");
                    selectedStructure = DataStructure::NONE;
                    isAtAlgorithms = false;
                } else {
                    state = AppState::MAIN_MENU;
                }
            }  

            if (selectedStructure == DataStructure::NONE) {
                selectedStructure = choiceMenu.getSelectedStructure();
                break;
            } else if (selectedStructure == DataStructure::LIST){
                choiceMenu.setLabels("Traversal", "Insertion Sort", "Merge Sort");
                isAtAlgorithms = true;
            } else if (selectedStructure == DataStructure::TREE){
                choiceMenu.setLabels("BFS", "Inorder Traversal", "Deletion");
                isAtAlgorithms = true;
            } else if (selectedStructure == DataStructure::GRAPH){
                choiceMenu.setLabels("Dijkstra", "Prim's", "Topological Sort");
                isAtAlgorithms = true;
            }

            if (selectedStructure == DataStructure::CUSTOM) {
                state = AppState::FILE_INPUT_MENU;
                break;
            }
            
            if (selectedAlgorithm == Algorithm::NONE) {
                selectedAlgorithm = choiceMenu.getSelectedAlgorithm();
            }

            if (selectedStructure != DataStructure::NONE && selectedAlgorithm != Algorithm::NONE) {
                state = AppState::POPULATE_MENU;
            }

            break;
        
        case AppState::POPULATE_MENU:
            populateMenu.update();
            if (populateMenu.shouldGoBack()) {
                state = AppState::CHOICE_MENU;
                selectedAlgorithm = Algorithm::NONE;
            }

            numbers = populateMenu.parseInput();

            if (!numbers.empty()) {
                state = AppState::VISUALIZATION_MENU;
                visualizationMenu.setNumbers(numbers);
                visualizationMenu.setSelectedStructure(selectedStructure);
                visualizationMenu.setSelectedAlgorithm(selectedAlgorithm);
            }

            break;
        
        case AppState::VISUALIZATION_MENU:
            visualizationMenu.update(); // temporary
            if (visualizationMenu.shouldGoBack()) {
                state = AppState::POPULATE_MENU;
                numbers.clear();
            }

            break;

        case AppState::FILE_INPUT_MENU: 
            fileInputMenu.update();
            if (fileInputMenu.shouldGoBack()) {
                isValidPath = false;
                path = "";
                state = AppState::CHOICE_MENU;
                selectedStructure = DataStructure::NONE;
            }

            if (fileInputMenu.shouldValidate()) {
                isValidPath = fileInputMenu.validateInput();
                if (isValidPath) {
                    path = fileInputMenu.getInput();
                }
            }

            if (isValidPath) {
                if (fileInputMenu.uploadFile()) {
                    state = AppState::POPULATE_MENU; // temporary
                    isValidPath = false;
                }
            }

            break;

        case AppState::EXIT:
            break;
    }
}

void AppController::draw() {
    switch (state) {
        case AppState::MAIN_MENU:
            mainMenu.draw();
            break;

        case AppState::CHOICE_MENU:
            choiceMenu.draw();
            break;

        case AppState::POPULATE_MENU:
            populateMenu.draw();
            break;
        
        case AppState::VISUALIZATION_MENU:
            visualizationMenu.draw(); // temporary
            break;

        case AppState::FILE_INPUT_MENU:
            fileInputMenu.draw();
            break;
        
        case AppState::EXIT:
            // nothing to draw; Exiting
            break;
    }
}

bool AppController::shouldClose() const {
    return WindowShouldClose() || state == AppState::EXIT;
}
