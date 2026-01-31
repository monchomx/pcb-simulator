// context.h
#ifndef CONTEXT_H
#define CONTEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef struct Context {
    SDL_Window *window;
    SDL_Renderer *renderer;
    int width, height;
    TTF_Font *defaultFont;
    int running;
} Context;

extern Context ctx;

int context_init(const char *title, int width, int height);
void context_destroy();

#endif
