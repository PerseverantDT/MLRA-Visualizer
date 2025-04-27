#pragma once

#include "MLRA/Core/RegisterCost.h"
#include "MLRA/Core/RegisterInstruction.h"

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct MLRA_Scenario_ MLRA_Scenario;

void MLRA_DestroyScenario(
    MLRA_Scenario *scenario
);

[[nodiscard]]
[[gnu::malloc, gnu::malloc(MLRA_DestroyScenario, 1)]]
MLRA_Scenario *MLRA_CreateScenario(
    size_t registerCount,
    MLRA_RegisterCost memorySpillCost
);

[[nodiscard, gnu::pure]]
[[gnu::nonnull(1), gnu::access(read_only, 1)]]
MLRA_RegisterCost MLRA_GetMemorySpillCostInScenario(
    MLRA_Scenario const *scenario
);

[[gnu::nonnull(1), gnu::access(read_write, 1)]]
void MLRA_SetMemorySpillCostInScenario(
    MLRA_Scenario *scenario,
    MLRA_RegisterCost memorySpillCost
);

[[nodiscard, gnu::pure]]
[[gnu::nonnull(1), gnu::access(read_only, 1)]]
size_t MLRA_GetRegisterCountInScenario(
    MLRA_Scenario const *scenario
);

[[gnu::nonnull(1), gnu::access(read_write, 1)]]
void MLRA_SetRegisterCountInScenario(
    MLRA_Scenario *scenario,
    size_t count
);

[[nodiscard, gnu::pure]]
[[gnu::nonnull(1), gnu::access(read_only, 1)]]
MLRA_RegisterCost MLRA_GetRegisterCostInScenario(
    MLRA_Scenario const *scenario,
    size_t index
);

[[gnu::nonnull(1), gnu::access(read_write, 1)]]
void MLRA_SetRegisterCostInScenario(
    MLRA_Scenario *scenario,
    size_t index,
    MLRA_RegisterCost registerCost
);

[[nodiscard, gnu::pure]]
[[gnu::nonnull(1), gnu::access(read_only, 1)]]
size_t MLRA_GetRegisterInstructionCountInScenario(
    MLRA_Scenario const *scenario
);

[[nodiscard, gnu::pure]]
[[gnu::nonnull(1), gnu::access(read_only, 1)]]
MLRA_RegisterInstruction MLRA_GetRegisterInstructionInScenario(
    MLRA_Scenario const *scenario,
    size_t index
);

[[gnu::nonnull(1), gnu::access(read_write, 1)]]
void MLRA_AppendRegisterInstructionToScenario(
    MLRA_Scenario *scenario,
    MLRA_RegisterInstruction instruction
);

[[gnu::nonnull(1), gnu::access(read_write, 1)]]
void MLRA_InsertRegisterInstructionToScenario(
    MLRA_Scenario *scenario,
    size_t index,
    MLRA_RegisterInstruction instruction
);

[[gnu::nonnull(1), gnu::access(read_write, 1)]]
void MLRA_RemoveRegisterInstructionBehindScenario(
    MLRA_Scenario *scenario
);

[[gnu::nonnull(1), gnu::access(read_write, 1)]]
void MLRA_RemoveRegisterInstructionAtScenario(
    MLRA_Scenario *scenario,
    size_t index
);

#ifdef __cplusplus
}
#endif
