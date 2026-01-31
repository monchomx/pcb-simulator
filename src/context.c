// context.c
#include "context.h"

Context ctx;

int context_init(const char *title, int width, int height) {
    ctx.window = NULL;
    ctx.renderer = NULL;
    ctx.defaultFont = NULL;
    ctx.width = width;
    ctx.height = height;
    ctx.running = 0;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) return 0;
    if (TTF_Init() < 0) {
        SDL_Quit();
        return 0;
    }

    ctx.window = SDL_CreateWindow(title,
                                  SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED,
                                  width, height,
                                  SDL_WINDOW_SHOWN);
    if (!ctx.window) {
        TTF_Quit();
        SDL_Quit();
        return 0;
    }

    ctx.renderer = SDL_CreateRenderer(ctx.window, -1, SDL_RENDERER_ACCELERATED);
    if (!ctx.renderer) {
        SDL_DestroyWindow(ctx.window);
        ctx.window = NULL;
        TTF_Quit();
        SDL_Quit();
        return 0;
    }

    ctx.defaultFont = TTF_OpenFont("./resources/fonts/arial.ttf", 24);
    if (!ctx.defaultFont) {
        SDL_DestroyRenderer(ctx.renderer);
        SDL_DestroyWindow(ctx.window);
        ctx.renderer = NULL;
        ctx.window = NULL;
        TTF_Quit();
        SDL_Quit();
        return 0;
    }

    ctx.running = 1;
    return 1;
}

void context_destroy() {
    TTF_CloseFont(ctx.defaultFont);
    SDL_DestroyRenderer(ctx.renderer);
    SDL_DestroyWindow(ctx.window);
    TTF_Quit();
    SDL_Quit();
}

