#ifndef PANEL_H
#define PANEL_H

#include "component.h"

// -------------------------------
// Definición de Panel
// -------------------------------
typedef struct Panel {
    Component base;   // hereda de Component
    // Overrides opcionales de color
    int use_custom_bg;
    int use_custom_border;
    Color custom_bg;
    Color custom_border;
} Panel;

// -------------------------------
// Funciones públicas
// -------------------------------

// Crear un Panel (constructor)
Panel* panel_create(int parentId, Position pos, Size size);

// Inicializar un Panel (si ya tienes la memoria reservada)
void panel_init(Panel *panel);

// Paint específico del Panel
int panel_paint(Component *self, SDL_Renderer *renderer);

// Update específico del Panel
int panel_update(Component *self, SDL_Event *event);

// Destruir un Panel (liberar memoria)
void panel_destroy(Panel *panel);

// Overrides de color
void panel_set_bg_color(Panel *panel, Color bg);
void panel_set_border_color(Panel *panel, Color border);
void panel_clear_bg_color(Panel *panel);
void panel_clear_border_color(Panel *panel);

#endif // PANEL_H
