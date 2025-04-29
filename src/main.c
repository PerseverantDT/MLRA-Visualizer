#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

int main(void)
{
    const int screenWidth = 960;
    const int screenHeight = 720;

    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "Minimum Local Register Allocation Visualizer");
    GuiLoadStyle("assets/styles/cyber/style_cyber.rgs");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(GetColor((unsigned int)GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

        EndDrawing();
    }
}
