#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
    int load;
    int store;
} MLRA_RegisterCost;

typedef struct MLRA_RegisterCostArray_ MLRA_RegisterCostArray;

[[gnu::access(read_write, 1)]]
void MLRA_DestroyRegisterCostArray(
    MLRA_RegisterCostArray *array
);

[[nodiscard]]
[[gnu::malloc, gnu::malloc(MLRA_DestroyRegisterCostArray, 1)]]
MLRA_RegisterCostArray *MLRA_CreateRegisterCostArray(
    size_t registerCount
);

[[nodiscard]]
MLRA_RegisterCostArray *MLRA_ResizeRegisterCostArray(
    MLRA_RegisterCostArray *array,
    size_t newRegisterCount
);

[[nodiscard, gnu::pure]]
[[gnu::nonnull(1), gnu::access(read_only, 1)]]
size_t MLRA_GetRegisterCostArraySize(
    MLRA_RegisterCostArray const *array
);

[[nodiscard, gnu::pure]]
[[gnu::nonnull(1), gnu::access(read_only, 1)]]
MLRA_RegisterCost MLRA_GetRegisterCostInArray(
    MLRA_RegisterCostArray const *array,
    size_t index
);

[[gnu::nonnull(1), gnu::access(read_write, 1)]]
void MLRA_SetRegisterCostInArray(
    MLRA_RegisterCostArray *array,
    size_t index,
    MLRA_RegisterCost registerCost
);

#ifdef __cplusplus
}
#endif
