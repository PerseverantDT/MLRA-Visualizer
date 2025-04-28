#include "MLRA/Core/Scenario.h"
#include "MLRA/Core/RegisterCost.h"
#include "MLRA/Core/RegisterInstruction.h"

#include <SDL3/SDL_log.h>

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

static void MLRA_WarnWhenMemorySpillCostIsSmallerThanRegisterCost(
    MLRA_RegisterCost const memorySpillCost,
    size_t const registerIndex,
    MLRA_RegisterCost const registerCost
)
{
    if (registerCost.load <= memorySpillCost.load && registerCost.store <= memorySpillCost.store) {
        return;
    }

    SDL_LogWarn(
        SDL_LOG_CATEGORY_APPLICATION,
        "Cost of spilling memory is not set higher than using register %llu. Using this scenario "
        "in solvers without modifying either the memory spill cost or register use cost will "
        "yield unexpected results.\n"
        "Memory spill cost: Load -> %d, Store -> %d\n"
        "Register %llu cost: Load -> %d, Store -> %d",
        (unsigned long long)registerIndex,
        memorySpillCost.load, memorySpillCost.store,
        (unsigned long long)registerIndex,
        registerCost.load, registerCost.store
    );
}

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
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "Could not create register cost array for scenario: %s",
            strerror(errno)
        );
        
        return nullptr;
    }

    MLRA_RegisterInstructionList *registerInstructions = MLRA_CreateRegisterInstructionList();
    if (registerInstructions == nullptr) {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "Could not create register instruction list for scenario: %s",
            strerror(errno)
        );

        MLRA_DestroyRegisterCostArray(registerCosts);
        return nullptr;
    }

    MLRA_Scenario *scenario = malloc(sizeof(MLRA_Scenario));
    if (scenario == nullptr) {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "Could not create scenario: %s",
            strerror(errno)
        );

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

    size_t registerCount = MLRA_GetRegisterCostArraySize(scenario->registerCosts);
    for (size_t index = 0; index < registerCount; ++index) {
        MLRA_RegisterCost registerCost = MLRA_GetRegisterCostInArray(scenario->registerCosts, index);
        MLRA_WarnWhenMemorySpillCostIsSmallerThanRegisterCost(
            memorySpillCost,
            index,
            registerCost
        );
    }

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
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION,
            "Could not change scenario register count: %s",
            strerror(errno)
        );

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

    MLRA_WarnWhenMemorySpillCostIsSmallerThanRegisterCost(
        scenario->memorySpillCost,
        index,
        registerCost
    );
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
