#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" 
{
#endif

typedef enum
{
    MLRA_RegisterInstructionType_Invalid,
    MLRA_RegisterInstructionType_Load,
    MLRA_RegisterInstructionType_Store
} MLRA_RegisterInstructionType;

typedef struct
{
    MLRA_RegisterInstructionType type;
    int virtualRegisterId;
} MLRA_RegisterInstruction;

typedef struct MLRA_RegisterInstructionList_ MLRA_RegisterInstructionList;

[[gnu::access(read_write, 1)]]
void MLRA_DestroyRegisterInstructionList(
    MLRA_RegisterInstructionList *list
);

[[nodiscard]]
[[gnu::malloc, gnu::malloc(MLRA_DestroyRegisterInstructionList)]]
MLRA_RegisterInstructionList *MLRA_CreateRegisterInstructionList(void);

[[nodiscard, gnu::pure]]
[[gnu::nonnull(1), gnu::access(read_only, 1)]]
size_t MLRA_GetRegisterInstructionCountInList(
    MLRA_RegisterInstructionList *list
);

[[nodiscard, gnu::pure]]
[[gnu::nonnull(1), gnu::access(read_only, 1)]]
MLRA_RegisterInstruction MLRA_GetRegisterInstructionInList(
    MLRA_RegisterInstructionList *list,
    size_t index
);

[[gnu::nonnull(1), gnu::access(read_write, 1)]]
void MLRA_AppendRegisterInstructionToList(
    MLRA_RegisterInstructionList *list,
    MLRA_RegisterInstruction instruction
);

[[gnu::nonnull(1), gnu::access(read_write, 1)]]
void MLRA_InsertRegisterInstructionAtList(
    MLRA_RegisterInstructionList *list,
    size_t index,
    MLRA_RegisterInstruction instruction
);

[[gnu::nonnull(1), gnu::access(read_write, 1)]]
void MLRA_RemoveRegisterInstructionBehindList(
    MLRA_RegisterInstructionList *list
);

[[gnu::nonnull(1), gnu::access(read_write, 1)]]
void MLRA_RemoveRegisterInstructionAtList(
    MLRA_RegisterInstructionList *list,
    size_t index
);

#ifdef __cplusplus
}
#endif
