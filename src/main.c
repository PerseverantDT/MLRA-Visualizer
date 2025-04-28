#include "MLRA/App/Application.h"
#include "MLRA/App/Log.h"

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_hints.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_ttf/SDL_textengine.h>

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#include <errno.h>
#include <stdlib.h>

[[gnu::cold]]
SDL_AppResult SDL_AppInit(
    void **const appstate,
    int const argc,
    char **const argv
)
{
    (void)argc;
    (void)argv;

    SDL_SetLogOutputFunction(&MLRA_LogOutput, nullptr);

    SDL_SetAppMetadataProperty(
        SDL_PROP_APP_METADATA_NAME_STRING,
        "Minimum Local Register Allocation Visualizer"
    );
    SDL_SetAppMetadataProperty(
        SDL_PROP_APP_METADATA_CREATOR_STRING,
        "Perseverant Determination"
    );
    SDL_SetAppMetadataProperty(
        SDL_PROP_APP_METADATA_IDENTIFIER_STRING,
        "io.gitlab.perseverantdt.mlra_visualizer"
    );
    SDL_SetAppMetadataProperty(
        SDL_PROP_APP_METADATA_TYPE_STRING,
        "application"
    );
    SDL_SetAppMetadataProperty(
        SDL_PROP_APP_METADATA_VERSION_STRING,
        "0.0.0"
    );
    SDL_SetAppMetadataProperty(
        SDL_PROP_APP_METADATA_COPYRIGHT_STRING,
        "Copyright (c) 2025 Perseverant Determination"
    );

    SDL_SetHint(SDL_HINT_WINDOWS_CLOSE_ON_ALT_F4, "1");
    #if !defined(NDEBUG)
    SDL_SetHint(SDL_HINT_RENDER_VULKAN_DEBUG, "1");
    #endif

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Could not initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_Window *window; // NOLINT(*-init-variables)
    SDL_Renderer *renderer; // NOLINT(*-init-variables)

    if (!SDL_CreateWindowAndRenderer(
        "Minimum Local Register Allocation Visualizer",
        960, // NOLINT(*-magic-numbers)
        720, // NOLINT(*-magic-numbers)
        SDL_WINDOW_VULKAN | SDL_WINDOW_HIDDEN,
        &window,
        &renderer
    )) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_PropertiesID rendererProps = SDL_GetRendererProperties(renderer);
    SDL_LogDebug(
        SDL_LOG_CATEGORY_APPLICATION,
        "Renderer driver: %s",
        SDL_GetStringProperty(rendererProps, SDL_PROP_RENDERER_NAME_STRING, "Unknown")
    );

    if (!TTF_Init()) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Could not initialize SDL_ttf: %s", SDL_GetError());
        
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        return SDL_APP_FAILURE;
    }

    TTF_TextEngine *textEngine = TTF_CreateRendererTextEngine(renderer);
    if (textEngine == nullptr) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Could not create text engine: %s", SDL_GetError());

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        return SDL_APP_FAILURE;
    }

    MLRA_Application *application = malloc(sizeof(MLRA_Application));
    if (application == nullptr) {
        SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Could not create application: %s", strerror(errno));

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        return SDL_APP_FAILURE;
    }

    application->window = window;
    application->renderer = renderer;
    application->textEngine = textEngine;

    *appstate = application;

    SDL_ShowWindow(window);

    return SDL_APP_CONTINUE;
}

[[gnu::hot]]
SDL_AppResult SDL_AppEvent(
    void *const appstate,
    SDL_Event *const event
)
{
    [[maybe_unused]] MLRA_Application *app = appstate;

    switch (event->type) {
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;
        default:
            return SDL_APP_CONTINUE;
    }
}

[[gnu::hot]]
SDL_AppResult SDL_AppIterate(
    void *const appstate
)
{
    MLRA_Application *app = appstate;

    SDL_RenderClear(app->renderer);

    SDL_RenderPresent(app->renderer);

    return SDL_APP_CONTINUE;
}

[[gnu::cold]]
void SDL_AppQuit(
    void *const appstate,
    SDL_AppResult const result
)
{
    (void)result;
    MLRA_Application *app = appstate;

    if (app != nullptr) {
        TTF_DestroyRendererTextEngine(app->textEngine);
        SDL_DestroyRenderer(app->renderer);
        SDL_DestroyWindow(app->window);

        free(app);
    }

    TTF_Quit();
    SDL_Quit();
}
