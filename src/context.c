// context.c
#include "context.h"

Context ctx;

int context_init(const char *title, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return 0;
    if (TTF_Init() < 0) return 0;

    ctx.window = SDL_CreateWindow(title,
                                  SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED,
                                  width, height,
                                  SDL_WINDOW_SHOWN);
    ctx.renderer = SDL_CreateRenderer(ctx.window, -1, SDL_RENDERER_ACCELERATED);
    ctx.width = width;
    ctx.height = height;
    ctx.defaultFont = TTF_OpenFont("./resources/fonts/arial.ttf", 24);
    ctx.running = 1;

    return ctx.window && ctx.renderer && ctx.defaultFont;
}

void context_destroy() {
    TTF_CloseFont(ctx.defaultFont);
    SDL_DestroyRenderer(ctx.renderer);
    SDL_DestroyWindow(ctx.window);
    TTF_Quit();
    SDL_Quit();
}

