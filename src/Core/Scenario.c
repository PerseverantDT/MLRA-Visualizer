#include "MLRA/Core/Scenario.h"
#include "MLRA/Core/RegisterCost.h"
#include "MLRA/Core/RegisterInstruction.h"

#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

struct MLRA_Scenario_
{
    MLRA_RegisterCostArray *registerCosts;
    MLRA_RegisterInstructionList *registerInstructions;
    MLRA_RegisterCost memorySpillCost;
};

void MLRA_DestroyScenario(
    MLRA_Scenario *const scenario
)
{
    if (scenario == nullptr) {
        return;
    }

    MLRA_DestroyRegisterCostArray(scenario->registerCosts);
    MLRA_DestroyRegisterInstructionList(scenario->registerInstructions);
    free(scenario);
}

[[nodiscard]]
[[gnu::malloc, gnu::malloc(MLRA_DestroyScenario, 1)]]
MLRA_Scenario *MLRA_CreateScenario(
    size_t const registerCount,
    MLRA_RegisterCost const memorySpillCost
)
{
    MLRA_RegisterCostArray *registerCosts = MLRA_CreateRegisterCostArray(registerCount);
    if (registerCosts == nullptr) {
        return nullptr;
    }

    MLRA_RegisterInstructionList *registerInstructions = MLRA_CreateRegisterInstructionList();
    if (registerInstructions == nullptr) {
        MLRA_DestroyRegisterCostArray(registerCosts);
        return nullptr;
    }

    MLRA_Scenario *scenario = malloc(sizeof(MLRA_Scenario));
    if (scenario == nullptr) {
        MLRA_DestroyRegisterInstructionList(registerInstructions);
        MLRA_DestroyRegisterCostArray(registerCosts);
        return nullptr;
    }
    
    scenario->registerCosts = registerCosts;
    scenario->registerInstructions = registerInstructions;
    scenario->memorySpillCost = memorySpillCost;

    return scenario;
}

[[nodiscard, gnu::pure]]
[[gnu::nonnull(1), gnu::access(read_only, 1)]]
MLRA_RegisterCost MLRA_GetMemorySpillCostInScenario(
    MLRA_Scenario const *const scenario
)
{
    return scenario->memorySpillCost;
}

[[gnu::nonnull(1), gnu::access(read_write, 1)]]
void MLRA_SetMemorySpillCostInScenario(
    MLRA_Scenario *const scenario,
    MLRA_RegisterCost const memorySpillCost
)
{
    assert(memorySpillCost.load > 0);
    assert(memorySpillCost.store > 0);

    scenario->memorySpillCost = memorySpillCost;
}

[[nodiscard, gnu::pure]]
[[gnu::nonnull(1), gnu::access(read_only, 1)]]
size_t MLRA_GetRegisterCountInScenario(
    MLRA_Scenario const *const scenario
)
{
    return MLRA_GetRegisterCostArraySize(scenario->registerCosts);
}

[[gnu::nonnull(1), gnu::access(read_write, 1)]]
void MLRA_SetRegisterCountInScenario(
    MLRA_Scenario *const scenario,
    size_t const count
)
{
    MLRA_RegisterCostArray *registerCosts = MLRA_ResizeRegisterCostArray(scenario->registerCosts, count);
    if (registerCosts == nullptr) {
        return;
    }

    scenario->registerCosts = registerCosts;
}

[[nodiscard, gnu::pure]]
[[gnu::nonnull(1), gnu::access(read_only, 1)]]
MLRA_RegisterCost MLRA_GetRegisterCostInScenario(
    MLRA_Scenario const *const scenario,
    size_t const index
)
{
    return MLRA_GetRegisterCostInArray(scenario->registerCosts, index);
}

[[gnu::nonnull(1), gnu::access(read_write, 1)]]
void MLRA_SetRegisterCostInScenario(
    MLRA_Scenario *const scenario,
    size_t const index,
    MLRA_RegisterCost const registerCost
)
{
    assert(registerCost.load > 0);
    assert(registerCost.store > 0);

    MLRA_SetRegisterCostInArray(scenario->registerCosts, index, registerCost);
}

[[nodiscard, gnu::pure]]
[[gnu::nonnull(1), gnu::access(read_only, 1)]]
size_t MLRA_GetRegisterInstructionCountInScenario(
    MLRA_Scenario const *const scenario
)
{
    return MLRA_GetRegisterInstructionCountInList(scenario->registerInstructions);
}

[[nodiscard, gnu::pure]]
[[gnu::nonnull(1), gnu::access(read_only, 1)]]
MLRA_RegisterInstruction MLRA_GetRegisterInstructionInScenario(
    MLRA_Scenario const *const scenario,
    size_t const index
)
{
    return MLRA_GetRegisterInstructionInList(scenario->registerInstructions, index);
}

[[gnu::nonnull(1), gnu::access(read_write, 1)]]
void MLRA_AppendRegisterInstructionToScenario(
    MLRA_Scenario *const scenario,
    MLRA_RegisterInstruction const instruction
)
{
    MLRA_AppendRegisterInstructionToList(scenario->registerInstructions, instruction);
}

[[gnu::nonnull(1), gnu::access(read_write, 1)]]
void MLRA_InsertRegisterInstructionToScenario(
    MLRA_Scenario *const scenario,
    size_t const index,
    MLRA_RegisterInstruction const instruction
)
{
    MLRA_InsertRegisterInstructionAtList(scenario->registerInstructions, index, instruction);
}

[[gnu::nonnull(1), gnu::access(read_write, 1)]]
void MLRA_RemoveRegisterInstructionBehindScenario(
    MLRA_Scenario *const scenario
)
{
    MLRA_RemoveRegisterInstructionBehindList(scenario->registerInstructions);
}

[[gnu::nonnull(1), gnu::access(read_write, 1)]]
void MLRA_RemoveRegisterInstructionAtScenario(
    MLRA_Scenario *const scenario,
    size_t const index
)
{
    MLRA_RemoveRegisterInstructionAtList(scenario->registerInstructions, index);
}
