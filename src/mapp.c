#include "mapp.h"

MAPP_Context* MAPP_Init(SDL_Window* window, SDL_Renderer* renderer) {
    MAPP_Context* ctx = malloc(sizeof(MAPP_Context));
    if (!ctx) return NULL;

    ctx->window = window;
    ctx->renderer = renderer;
    ctx->inputFlags = 0;
    ctx->zoomLevel = 100;
    ctx->wheelValue = 0;
    
    // Inicializar coordenadas del mouse
    ctx->mouseCoords.startX = 0;
    ctx->mouseCoords.startY = 0;
    ctx->mouseCoords.endX = 0;
    ctx->mouseCoords.endY = 0;
    ctx->mouseCoords.currentX = 0;
    ctx->mouseCoords.currentY = 0;

    return ctx;
}

void MAPP_Destroy(MAPP_Context* ctx) {
    if (ctx) {
        // liberar texturas, otros recursos, etc.
        free(ctx);
    }
}

void MAPP_HandleEvent(MAPP_Context* ctx, SDL_Event* event){
    MAPP_ProcessInput(ctx,event);
    MAPP_UpdateStatus(ctx);
    MAPP_UpdateRender(ctx);
}

void MAPP_ProcessInput(MAPP_Context* ctx, SDL_Event* event){
    if(SDL_IS_KEY_EVENT(event->type)){//Evento de teclado
        MAPP_ProcessKBEvent(ctx, event);
    }else if (SDL_IS_MOUSE_EVENT(event->type)){//Evento de Mouse
        MAPP_ProcessMSEvent(ctx, event);
    }
}

void MAPP_ProcessKBEvent(MAPP_Context* ctx, SDL_Event* event){
    SDL_Keycode sym = event->key.keysym.sym;

    if(event->type == SDL_KEYDOWN){
        if(SDL_IS_CTRL_KEY(sym)){
            ctx->inputFlags |= FLAG_CTRL_DOWN;
            ctx->inputFlags &= ~FLAG_CTRL_UP;
        }
    }else if(event->type == SDL_KEYUP){
        if(SDL_IS_CTRL_KEY(sym)){
            ctx->inputFlags |= FLAG_CTRL_UP;
            ctx->inputFlags &= ~FLAG_CTRL_DOWN;
        }
    }
}

void MAPP_ProcessMSEvent(MAPP_Context* ctx,SDL_Event* event){
    if(event->type == SDL_MOUSEBUTTONDOWN){
        if(SDL_IS_LEFT_MSBTN(event->button.button)){
            ctx->inputFlags |= FLAG_LEFT_MSBTN_DOWN;
            ctx->inputFlags &= ~FLAG_LEFT_MSBTN_UP;
            ctx->mouseCoords.startX = event->button.x;
            ctx->mouseCoords.startY = event->button.y;
        }else if (SDL_IS_RIGHT_MSBTN(event->button.button)){
            ctx->inputFlags |= FLAG_RIGHT_MSBTN_DOWN;
            ctx->inputFlags &= ~FLAG_RIGHT_MSBTN_UP;
            ctx->mouseCoords.startX = event->button.x;
            ctx->mouseCoords.startY = event->button.y;
        }
    }else if(event->type == SDL_MOUSEBUTTONUP){
        if(SDL_IS_LEFT_MSBTN(event->button.button)){
            ctx->inputFlags |= FLAG_LEFT_MSBTN_UP;
            ctx->inputFlags &= ~FLAG_LEFT_MSBTN_DOWN;
            ctx->mouseCoords.endX = event->button.x;
            ctx->mouseCoords.endY = event->button.y;
        }else if (SDL_IS_RIGHT_MSBTN(event->button.button)){
            ctx->inputFlags |= FLAG_RIGHT_MSBTN_UP;
            ctx->inputFlags &= ~FLAG_RIGHT_MSBTN_DOWN;
            ctx->mouseCoords.endX = event->button.x;
            ctx->mouseCoords.endY = event->button.y;
        }
    }else if(event->type == SDL_MOUSEMOTION){
        ctx->inputFlags |= FLAG_MOUSE_MOTION; //Se apaga en otro lugar
        ctx->mouseCoords.currentX = event->motion.x;
        ctx->mouseCoords.currentY = event->motion.y;
    }else if(event->type == SDL_MOUSEWHEEL){
        ctx->inputFlags |= FLAG_MOUSE_WHEEL; //Se apaga en otro lugar
        ctx->wheelValue = event->wheel.y; // positivo arriba, negativo abajo
    }
}

//Pendiente de implementar
void MAPP_UpdateStatus(MAPP_Context* ctx) {
    (void)ctx;
}

//Pendiente de implementar
void MAPP_UpdateRender(MAPP_Context* ctx) {
    (void)ctx;
}
