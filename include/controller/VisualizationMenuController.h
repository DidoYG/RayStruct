// include/controller/VisualizationMenuController.h
#pragma once
#include "controller/ButtonController.h"
#include "view/VisualizationMenuView.h"
#include "controller/ChoiceMenuController.h"

class VisualizationMenuController {
    private:
        std::vector<int> numbers;
        DataStructure selectedStructure;
        Algorithm selectedAlgorithm;

        VisualizationMenuView view;

        ButtonModel backButtonModel;

        ButtonController backButton;
    
    public:
        VisualizationMenuController();

        void setNumbers(const std::vector<int>& nums);
        void setSelectedStructure(DataStructure ds);
        void setSelectedAlgorithm(Algorithm alg);

        void update();
        void draw();

        bool shouldGoBack();
};