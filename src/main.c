#include <raylib.h>
#include <raygui.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

[[gnu::nonnull(1), gnu::access(read_write, 1)]]
[[gnu::nonnull(2), gnu::access(read_write, 2)]]
static void DrawEditRegisterCountDialogBox(bool *visible, size_t *registerCount)
{
    static bool lastVisible = false;
    static char buffer[16];

    if (!(*visible)) {
        lastVisible = false;    
        return;
    }

    if (!lastVisible) {
        memset(buffer, 0, sizeof(char) * 16);
    }

    int pressedButton = GuiTextInputBox(
        (Rectangle){ 330, 260, 300, 200 },
        "Edit Register Count",
        "Set the number of registers to use for the scenario. Minimum of 1.",
        "Cancel;Apply",
        buffer,
        15,
        nullptr
    );

    if (pressedButton == 2) {
        size_t result = strtoull(buffer, nullptr, 10);
        if (result > 0) {
            *registerCount = result;
        }
    }

    if (pressedButton != -1) {
        *visible = false;
    }

    lastVisible = true;
}

[[gnu::nonnull(2), gnu::access(read_write, 2)]]
static void DrawRegisterCount(size_t registerCount, bool *editRegisterCount)
{
    static int posX = 20;
    static int posY = 20;
    DrawText(
        "Number of Registers: ",
        posX,
        posY,
        20,
        GetColor((unsigned int)GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL))
    );
    DrawText(
        TextFormat("%zu", registerCount),
        posX + 224,
        posY,
        20,
        GetColor((unsigned int)GuiGetStyle(DEFAULT, *editRegisterCount ? TEXT_COLOR_FOCUSED : TEXT_COLOR_NORMAL))
    );
    if (!(*editRegisterCount)) {
        *editRegisterCount = GuiButton(
            (Rectangle){(float)posX + 224.0F, (float)posY + 25.0F, 40, 20},
            "Edit"
        );
    }
}

int main(void)
{
    const int screenWidth = 960;
    const int screenHeight = 720;

    size_t registerCount = 8;

    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "Minimum Local Register Allocation Visualizer");
    GuiLoadStyle("assets/styles/cyber/style_cyber.rgs");

    bool editRegisterCount = false;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(GetColor((unsigned int)GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

        DrawRegisterCount(registerCount, &editRegisterCount);
        DrawEditRegisterCountDialogBox(&editRegisterCount, &registerCount);

        EndDrawing();
    }
}
