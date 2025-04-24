#pragma once

#include <SDL3/SDL_log.h>

#ifdef __cplusplus
extern "C"
{
#endif

[[gnu::nonnull(4), gnu::access(read_only, 4)]]
void MLRA_LogOutput(
    void *userdata,
    int category,
    SDL_LogPriority priority,
    char const *message
);

#ifdef __cplusplus
}
#endif
