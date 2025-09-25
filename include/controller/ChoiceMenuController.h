// include/controller/ChoiceMenuController.h
#pragma once
#include "controller/ButtonController.h"
#include "view/ChoiceMenuView.h"

enum class DataStructure {
    NONE,
    LIST,
    TREE,
    GRAPH,
    CUSTOM
};

// temporary algorithms
enum class Algorithm {
    NONE,
    TRAVERSAL,
    INSERTION_SORT,
    MERGE_SORT,
    BFS,
    INORDER,
    DELETION,
    DIJKSTRA,
    PRIMS,
    TOPOLOGICAL_SORT,
    CUSTOM
};

class ChoiceMenuController {
private:
    std::string label1, label2, label3;

    DataStructure selectedStructure;

    ChoiceMenuView view;

    ButtonModel backButtonModel;
    ButtonModel optionOneButtonModel;
    ButtonModel optionTwoButtonModel;
    ButtonModel optionThreeButtonModel;
    ButtonModel customButtonModel;

    ButtonController backButton;
    ButtonController optionOneButton;
    ButtonController optionTwoButton;
    ButtonController optionThreeButton;
    ButtonController customButton;

public:
    ChoiceMenuController();

    void setLabels(const std::string& label1, const std::string& label2, const std::string& label3);  

    void update();
    void draw();

    bool shouldGoBack() const;
    DataStructure getSelectedStructure();
    Algorithm getSelectedAlgorithm() const;
};