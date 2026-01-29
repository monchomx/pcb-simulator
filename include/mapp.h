#ifndef MAPP_H
#define MAPP_H

#include <SDL2/SDL.h>

// Teclado
#define SDL_IS_KEY_EVENT(t)   ((t) >= SDL_KEYDOWN && (t) <= SDL_KEYUP)
// Mouse
#define SDL_IS_MOUSE_EVENT(t) ((t) >= SDL_MOUSEMOTION && (t) <= SDL_MOUSEWHEEL)
// Joystick
#define SDL_IS_JOYSTICK_EVENT(t) ((t) >= SDL_JOYAXISMOTION && (t) <= SDL_JOYDEVICEADDED)
// GameController
#define SDL_IS_CONTROLLER_EVENT(t) ((t) >= SDL_CONTROLLERAXISMOTION && (t) <= SDL_CONTROLLERDEVICEREMOVED)
// Ventana
#define SDL_IS_WINDOW_EVENT(t) ((t) == SDL_WINDOWEVENT)

#define SDL_IS_CTRL_KEY(sym) ((sym) == SDLK_LCTRL || (sym) == SDLK_RCTRL)
#define SDL_IS_LEFT_MSBTN(btn) ((btn) == SDL_BUTTON_LEFT)
#define SDL_IS_RIGHT_MSBTN(btn) ((btn) == SDL_BUTTON_RIGHT)

#define FLAG_CTRL_DOWN 0x01
#define FLAG_CTRL_UP   0x02 
#define FLAG_LEFT_MSBTN_DOWN 0x04
#define FLAG_RIGHT_MSBTN_DOWN 0x08
#define FLAG_LEFT_MSBTN_UP 0x10
#define FLAG_RIGHT_MSBTN_UP 0x20
#define FLAG_MOUSE_MOTION 0x40
#define FLAG_MOUSE_WHEEL 0x80

typedef struct {
    int startX;
    int startY;
    int endX;
    int endY;
    int currentX;
    int currentY;
} MAPP_MouseCoords;

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    MAPP_MouseCoords mouseCoords;
    unsigned int inputFlags;
    int zoomLevel;
    int wheelValue;
    // otros estados globales
} MAPP_Context;


MAPP_Context* MAPP_Init(SDL_Window* window, SDL_Renderer* renderer);
void MAPP_Destroy(MAPP_Context* ctx);

void MAPP_UpdateRender(MAPP_Context* ctx);
void MAPP_UpdateStatus(MAPP_Context* ctx);
void MAPP_ProcessKBEvent(MAPP_Context* ctx, SDL_Event* event);
void MAPP_ProcessMSEvent(MAPP_Context* ctx, SDL_Event* event);
void MAPP_ProcessInput(MAPP_Context* ctx, SDL_Event* event);
void MAPP_HandleEvent(MAPP_Context* ctx,SDL_Event* event);

#endif 