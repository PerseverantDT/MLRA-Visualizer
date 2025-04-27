#include "MLRA/App/Log.h"

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_messagebox.h>
#include <SDL3/SDL_log.h>

__attribute__((nonnull(4)))
void MLRA_LogOutput(
    void *const userdata,
    int const category,
    SDL_LogPriority const priority,
    char const *const message
)
{
    (void)userdata;

    if (priority == SDL_LOG_PRIORITY_CRITICAL) {
        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR,
            SDL_GetAppMetadataProperty(SDL_PROP_APP_METADATA_NAME_STRING),
            message,
            nullptr
        );
    }

    SDL_GetDefaultLogOutputFunction()(userdata, category, priority, message);
}
