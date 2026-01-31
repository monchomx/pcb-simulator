#ifndef LABEL_H
#define LABEL_H

#include "component.h"

// -------------------------------
// Definición de Label
// -------------------------------
typedef struct Label {
    Component base;   // hereda de Component
    char *text;
    TTF_Font *font;
} Label;

// -------------------------------
// Funciones públicas
// -------------------------------

// Crear un Label
Label* label_create(int parentId, Position pos, Size size, const char *txt, TTF_Font *font);

// Inicializar un Label
void label_init(Label *lbl);

// Update del Label (no consume eventos, solo propaga)
int label_update(Component *self, SDL_Event *event);

// Paint del Label
int label_paint(Component *self, SDL_Renderer *renderer);

// Destroy del Label
void label_destroy(Label *lbl);

#endif // LABEL_H
