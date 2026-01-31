#ifndef BUTTON_H
#define BUTTON_H

#include "component.h"

// -------------------------------
// Estados del botón
// -------------------------------
typedef enum {
    BTN_NORMAL,
    BTN_HOVER,
    BTN_PRESSED
} ButtonState;

// -------------------------------
// Definición de Button
// -------------------------------
typedef struct Button {
    Component base;       // hereda de Component
    ButtonState state;

    // Colores por estado
    Color normalBg;
    Color hoverBg;
    Color pressedBg;

    Color normalFg;
    Color hoverFg;
    Color pressedFg;

    char *text;
    TTF_Font *font;

    // Callback principal
    void (*onClick)(struct Button *self, SDL_Event *event);
} Button;

// -------------------------------
// Funciones públicas
// -------------------------------
Button* button_create(int parentId, Position pos, Size size, const char *txt, TTF_Font *font);
void button_init(Button *btn);
int  button_update(Component *self, SDL_Event *event);
int button_paint(Component *self, SDL_Renderer *renderer);
void button_destroy(Button *btn);

#endif // BUTTON_H
