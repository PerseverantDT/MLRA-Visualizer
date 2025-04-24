#pragma once

#include <SDL3/SDL_video.h>
#include <SDL3/SDL_render.h>

#include <SDL3_ttf/SDL_textengine.h>

typedef struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_TextEngine *textEngine;
} MLRA_Application;
