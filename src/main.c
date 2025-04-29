#include "MLRA/Core/Scenario.h"

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
        "Set the number of registers to use for\nthe scenario. Minimum of 1.",
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

[[gnu::nonnull(3), gnu::access(read_write, 3)]]
static void DrawRegisterCount(size_t registerCount, bool showEditButton, bool *editRegisterCount)
{
    static int posX = 20;
    static int posY = 70;
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
    if (showEditButton) {
        *editRegisterCount = GuiButton(
            (Rectangle){(float)posX + 274.0F, (float)posY, 40, 20},
            "Edit"
        );
    }
}

[[gnu::nonnull(3), gnu::access(read_write, 3)]]
[[gnu::nonnull(5), gnu::access(read_write, 5)]]
static void DrawMemorySpillCost(
    MLRA_RegisterCost memorySpillCost,
    bool showLoadEditButton,
    bool *editMemorySpillLoadCost,
    bool showStoreEditButton,
    bool *editMemorySpillStoreCost
)
{
    static int posX = 20;
    static int posY = 100;
    static int storePosXOffset = 120;

    DrawText(
        "Memory Spill Cost: ",
        posX,
        posY,
        20,
        GetColor((unsigned int)GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL))
    );
    DrawText(
        TextFormat("Load: %d", memorySpillCost.load),
        posX,
        posY + 25,
        20,
        GetColor((unsigned int)GuiGetStyle(DEFAULT, *editMemorySpillLoadCost ? TEXT_COLOR_FOCUSED : TEXT_COLOR_NORMAL))
    );
    if (showLoadEditButton) {
        *editMemorySpillLoadCost = GuiButton(
            (Rectangle){(float)posX, (float)posY + 50.0F, 40, 20},
            "Edit"
        );
    }
    DrawText(
        TextFormat("Store: %d", memorySpillCost.store),
        posX + storePosXOffset,
        posY + 25,
        20,
        GetColor((unsigned int)GuiGetStyle(DEFAULT, *editMemorySpillStoreCost ? TEXT_COLOR_FOCUSED : TEXT_COLOR_NORMAL))
    );
    if (showStoreEditButton) {
        *editMemorySpillStoreCost = GuiButton(
            (Rectangle){(float)(posX + storePosXOffset), (float)posY + 50.0F, 40, 20},
            "Edit"
        );
    }
}

[[gnu::nonnull(1), gnu::access(read_write, 1)]]
[[gnu::nonnull(2), gnu::access(read_write, 2)]]
static void DrawEditMemorySpillLoadCostDialogBox(
    bool *visible,
    MLRA_Scenario *scenario
)
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
        "Edit Memory Spill Cost",
        "Set the cost of loading a value from memory.",
        "Cancel;Apply",
        buffer,
        15,
        nullptr
    );

    if (pressedButton == 2) {
        int result = atoi(buffer);
        if (result >= 0) {
            MLRA_RegisterCost memorySpillCost = MLRA_GetMemorySpillCostInScenario(scenario);
            memorySpillCost.load = result;
            MLRA_SetMemorySpillCostInScenario(scenario, memorySpillCost);
        }
    }

    if (pressedButton != -1) {
        *visible = false;
    }

    lastVisible = true;
}

[[gnu::nonnull(1), gnu::access(read_write, 1)]]
[[gnu::nonnull(2), gnu::access(read_write, 2)]]
static void DrawEditMemorySpillStoreCostDialogBox(
    bool *visible,
    MLRA_Scenario *scenario
)
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
        "Edit Memory Spill Cost",
        "Set the cost of storing a value to memory.",
        "Cancel;Apply",
        buffer,
        15,
        nullptr
    );

    if (pressedButton == 2) {
        int result = atoi(buffer);
        if (result >= 0) {
            MLRA_RegisterCost memorySpillCost = MLRA_GetMemorySpillCostInScenario(scenario);
            memorySpillCost.store = result;
            MLRA_SetMemorySpillCostInScenario(scenario, memorySpillCost);
        }
    }

    if (pressedButton != -1) {
        *visible = false;
    }

    lastVisible = true;
}

int main(void)
{
    const int screenWidth = 960;
    const int screenHeight = 720;

    size_t registerCount = 8;
    MLRA_Scenario *scenario = MLRA_CreateScenario(registerCount, (MLRA_RegisterCost){5, 5});
    if (scenario == nullptr) {
        return 1;
    }

    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "Minimum Local Register Allocation Visualizer");
    GuiLoadStyle("assets/styles/cyber/style_cyber.rgs");

    bool showEditRegisterCountButton = true;
    bool editRegisterCount = false;

    bool showEditMemorySpillLoadCostButton = true;
    bool editMemorySpillLoadCost = false;
    bool showEditMemorySpillStoreCostButton = true;
    bool editMemorySpillStoreCost = false;

    while (!WindowShouldClose()) {
        if (editRegisterCount || editMemorySpillLoadCost || editMemorySpillStoreCost) {
            showEditRegisterCountButton = false;
            showEditMemorySpillLoadCostButton = false;
            showEditMemorySpillStoreCostButton = false;
        }
        else {
            showEditRegisterCountButton = true;
            showEditMemorySpillLoadCostButton = true;
            showEditMemorySpillStoreCostButton = true;
        }

        BeginDrawing();
        ClearBackground(GetColor((unsigned int)GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

        DrawRegisterCount(registerCount, showEditRegisterCountButton, &editRegisterCount);
        DrawEditRegisterCountDialogBox(&editRegisterCount, &registerCount);
        DrawMemorySpillCost(
            MLRA_GetMemorySpillCostInScenario(scenario),
            showEditMemorySpillLoadCostButton,
            &editMemorySpillLoadCost,
            showEditMemorySpillStoreCostButton,
            &editMemorySpillStoreCost
        );
        DrawEditMemorySpillLoadCostDialogBox(&editMemorySpillLoadCost, scenario);
        DrawEditMemorySpillStoreCostDialogBox(&editMemorySpillStoreCost, scenario);

        EndDrawing();
    }
}
