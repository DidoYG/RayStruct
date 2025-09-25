// main.cpp
#include "raylib.h"
#include "controller/AppController.h"

int main() {
    // Get the current monitor's width and height
    int display = GetCurrentMonitor();
    int screenWidth = GetMonitorWidth(display);
    int screenHeight = GetMonitorHeight(display);

    // Initialize the window
    InitWindow(screenWidth, screenHeight, "RayStruct++");
    SetTargetFPS(60);

    AppController app;

    while (!app.shouldClose()) {
        // UPDATE
        app.update();

        // DRAW
        BeginDrawing();
        ClearBackground(RAYWHITE);
        app.draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
