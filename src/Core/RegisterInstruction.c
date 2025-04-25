#include "MLRA/Core/RegisterInstruction.h"

#include <SDL3/SDL_stdinc.h>

#include <assert.h>
#include <limits.h>
#include <stddef.h>
#include <string.h>

struct MLRA_RegisterInstructionList_
{
    MLRA_RegisterInstruction *instructions;
    size_t count;
    size_t capacity;
};

[[gnu::access(read_write, 1)]]
void MLRA_DestroyRegisterInstructionList(
    MLRA_RegisterInstructionList *list
)
{
    if (list == nullptr) {
        return;
    }

    if (list->instructions != nullptr) {
        SDL_free(list->instructions);
    }

    SDL_free(list);
}

[[nodiscard]]
[[gnu::malloc, gnu::malloc(MLRA_DestroyRegisterInstructionList)]]
MLRA_RegisterInstructionList *MLRA_CreateRegisterInstructionList(void)
{
    MLRA_RegisterInstructionList *list = SDL_malloc(sizeof(MLRA_RegisterInstructionList));
    if (list == nullptr) {
        return nullptr;
    }

    list->instructions = nullptr;
    list->count = 0;
    list->capacity = 0;

    return list;
}

[[nodiscard, gnu::pure]]
[[gnu::nonnull(1), gnu::access(read_only, 1)]]
size_t MLRA_GetRegisterInstructionCountInList(
    MLRA_RegisterInstructionList *list
)
{
    return list->count;
}

[[nodiscard, gnu::pure]]
[[gnu::nonnull(1), gnu::access(read_only, 1)]]
MLRA_RegisterInstruction MLRA_GetRegisterInstructionInList(
    MLRA_RegisterInstructionList *list,
    size_t index
)
{
    assert(index < list->count);

    return list->instructions[index];
}

[[gnu::nonnull(1), gnu::access(read_write, 1)]]
void MLRA_AppendRegisterInstructionToList(
    MLRA_RegisterInstructionList *list,
    MLRA_RegisterInstruction instruction
)
{
    assert(
        instruction.type == MLRA_RegisterInstructionType_Load 
        || instruction.type == MLRA_RegisterInstructionType_Store
    );

    if (list->instructions == nullptr) {
        list->instructions = SDL_malloc(8 * sizeof(MLRA_RegisterInstruction));
        if (list->instructions == nullptr) {
            return;
        }

        list->capacity = 8;
    }
    else if (list->count == list->capacity) {
        size_t oldCapacity = list->capacity;
        size_t totalSize;
        if (__builtin_mul_overflow(list->capacity * 2, sizeof(MLRA_RegisterInstruction), &totalSize)) {
            totalSize = SIZE_MAX;
        }

        size_t expectedCapacity = totalSize / sizeof(MLRA_RegisterInstruction);
        if (oldCapacity == expectedCapacity) {
            return;
        }

        MLRA_RegisterInstruction *instructions = SDL_realloc(list->instructions, totalSize);
        if (instructions == nullptr) {
            return;
        }

        list->capacity = expectedCapacity;
        list->instructions = instructions;
    }

    list->instructions[list->count] = instruction;
    list->count++;
}

[[gnu::nonnull(1), gnu::access(read_write, 1)]]
void MLRA_InsertRegisterInstructionAtList(
    MLRA_RegisterInstructionList *list,
    size_t index,
    MLRA_RegisterInstruction instruction
)
{
    assert(
        instruction.type == MLRA_RegisterInstructionType_Load 
        || instruction.type == MLRA_RegisterInstructionType_Store
    );
    assert(index <= list->count);

    if (list->instructions == nullptr) {
        list->instructions = SDL_malloc(8 * sizeof(MLRA_RegisterInstruction));
        if (list->instructions == nullptr) {
            return;
        }

        list->capacity = 8;
    }
    else if (list->count == list->capacity) {
        size_t oldCapacity = list->capacity;
        size_t totalSize;
        if (__builtin_mul_overflow(list->capacity * 2, sizeof(MLRA_RegisterInstruction), &totalSize)) {
            totalSize = SIZE_MAX;
        }

        size_t expectedCapacity = totalSize / sizeof(MLRA_RegisterInstruction);
        if (oldCapacity == expectedCapacity) {
            return;
        }

        MLRA_RegisterInstruction *instructions = SDL_realloc(list->instructions, totalSize);
        if (instructions == nullptr) {
            return;
        }

        list->capacity = expectedCapacity;
        list->instructions = instructions;
    }

    if (index == list->count) {
        list->instructions[list->count] = instruction;
    }
    else {
        memmove(list->instructions + index, list->instructions + index + 1, sizeof(MLRA_RegisterInstruction) * (list->count - index));
        list->instructions[index] = instruction;
    }

    ++list->count;
}

[[gnu::nonnull(1), gnu::access(read_write, 1)]]
void MLRA_RemoveRegisterInstructionBehindList(
    MLRA_RegisterInstructionList *list
)
{
    if (list->count == 0) {
        return;
    }

    --list->count;

    size_t spaceThreshold;
    if (!__builtin_mul_overflow(list->count, 3, &spaceThreshold)) {
        if (spaceThreshold > list->capacity) {
            MLRA_RegisterInstruction *instructions = SDL_realloc(list->instructions, sizeof(MLRA_RegisterInstruction) * list->count * 2);
            if (instructions == nullptr) {
                return;
            }

            list->instructions = instructions;
            list->capacity = list->count * 2;
        }
    }
}

[[gnu::nonnull(1), gnu::access(read_write, 1)]]
void MLRA_RemoveRegisterInstructionAtList(
    MLRA_RegisterInstructionList *list,
    size_t index
)
{
    assert(index < list->count);

    if (list->count == 0) {
        return;
    }

    if (index != list->count - 1) {
        memmove(list->instructions + index, list->instructions + index + 1, sizeof(MLRA_RegisterInstruction) * (list->count - index - 1));
    }

    --list->count;

    size_t spaceThreshold;
    if (!__builtin_mul_overflow(list->count, 3, &spaceThreshold)) {
        if (spaceThreshold > list->capacity) {
            MLRA_RegisterInstruction *instructions = SDL_realloc(list->instructions, sizeof(MLRA_RegisterInstruction) * list->count * 2);
            if (instructions == nullptr) {
                return;
            }

            list->instructions = instructions;
            list->capacity = list->count * 2;
        }
    }
}
