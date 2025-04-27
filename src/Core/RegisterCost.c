#include "MLRA/Core/RegisterCost.h"

#include <SDL3/SDL_stdinc.h>

#include <assert.h>
#include <stddef.h>

struct MLRA_RegisterCostArray_
{
    size_t count;
    [[gnu::counted_by(count)]] MLRA_RegisterCost costs[];
};

[[gnu::access(read_write, 1)]]
void MLRA_DestroyRegisterCostArray(
    MLRA_RegisterCostArray *const array
)
{
    if (array == nullptr) {
        return;
    }

    SDL_free(array);
}

[[nodiscard]]
[[gnu::malloc, gnu::malloc(MLRA_DestroyRegisterCostArray, 1)]]
MLRA_RegisterCostArray *MLRA_CreateRegisterCostArray(
    size_t const registerCount
)
{
    size_t costsSize;
    if (__builtin_mul_overflow(registerCount, sizeof(MLRA_RegisterCost), &costsSize)) {
        return nullptr;
    }

    size_t totalSize;
    if (__builtin_add_overflow(sizeof(MLRA_RegisterCostArray), costsSize, &totalSize)) {
        return nullptr;
    }

    MLRA_RegisterCostArray *array = SDL_malloc(totalSize);
    if (array == nullptr) {
        return nullptr;
    }

    array->count = registerCount;
    for (size_t index = 0; index < registerCount; ++index) {
        array->costs[index] = (MLRA_RegisterCost){1, 1};
    }

    return array;
}

[[nodiscard]]
MLRA_RegisterCostArray *MLRA_ResizeRegisterCostArray(
    MLRA_RegisterCostArray *const array,
    size_t const newRegisterCount
)
{
    size_t costsSize;
    if (__builtin_mul_overflow(newRegisterCount, sizeof(MLRA_RegisterCost), &costsSize)) {
        return nullptr;
    }

    size_t totalSize;
    if (__builtin_add_overflow(sizeof(MLRA_RegisterCostArray), costsSize, &totalSize)) {
        return nullptr;
    }

    size_t oldCount = array == nullptr ? 0 : array->count;
    MLRA_RegisterCostArray *newArray = SDL_realloc(array, totalSize);
    if (newArray == nullptr) {
        return nullptr;
    }

    newArray->count = newRegisterCount;
    if (newRegisterCount > oldCount) {
        for (size_t index = oldCount; index < newRegisterCount; ++index) {
            newArray->costs[index] = (MLRA_RegisterCost){1, 1};
        }
    }

    return newArray;
}

[[nodiscard, gnu::pure]]
[[gnu::nonnull(1), gnu::access(read_only, 1)]]
size_t MLRA_GetRegisterCostArraySize(
    MLRA_RegisterCostArray const *const array
)
{
    return array->count;
}

[[nodiscard, gnu::pure]]
[[gnu::nonnull(1), gnu::access(read_only, 1)]]
MLRA_RegisterCost MLRA_GetRegisterCostInArray(
    MLRA_RegisterCostArray const *const array,
    size_t const index
)
{
    assert(index < array->count);
    
    return array->costs[index];
}

[[gnu::nonnull(1), gnu::access(read_write, 1)]]
void MLRA_SetRegisterCostInArray(
    MLRA_RegisterCostArray *const array,
    size_t const index,
    MLRA_RegisterCost const registerCost
)
{
    assert(index < array->count);
    assert(registerCost.load > 0);
    assert(registerCost.store > 0);

    array->costs[index] = registerCost;
}
