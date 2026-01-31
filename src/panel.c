#include "panel.h"
#include <stdlib.h>

// -------------------------------
// Crear un Panel
// -------------------------------
Panel* panel_create(int parentId, Position pos, Size size) {
    Panel *panel = (Panel*)malloc(sizeof(Panel));
    if (!panel) return NULL;
    
    panel_init(panel);
    panel->base.pos = pos;
    panel->base.size = size;
    
    // Usar helper para vincular al padre
    component_attach_to_parent(&panel->base, parentId);
    
    return panel;
}

// -------------------------------
// Inicializar un Panel
// -------------------------------
void panel_init(Panel *panel) {
    component_init(&panel->base);

    panel->use_custom_bg = 0;
    panel->use_custom_border = 0;
    panel->custom_bg = (Color){0, 0, 0, 0};
    panel->custom_border = (Color){0, 0, 0, 0};

    // Sobrescribir punteros de función con los específicos del Panel
    panel->base.paint = panel_paint;
    panel->base.update = panel_update;
    panel->base.destroy = (void (*)(Component*))panel_destroy;
}

// -------------------------------
// Update del Panel
// -------------------------------
int panel_update(Component *self, SDL_Event *event) {
    // El Panel no consume eventos, solo propaga a hijos
    return component_propagate_event_to_children(self, event);
}

// -------------------------------
// Paint del Panel
// -------------------------------
int panel_paint(Component *self, SDL_Renderer *renderer) {
    if (!self->visible) return 0;

    Panel *panel = (Panel*)self;

    SDL_Rect dstRect = { self->pos.x, self->pos.y, self->size.width, self->size.height };

    // Fondo
    Color bg = panel->use_custom_bg ? panel->custom_bg : currentTheme.panel_bg;
    SDL_SetRenderDrawColor(renderer,
                           bg.r,
                           bg.g,
                           bg.b,
                           bg.a);
    SDL_RenderFillRect(renderer, &dstRect);

    // Borde
    Color border = panel->use_custom_border ? panel->custom_border : currentTheme.panel_border;
    SDL_SetRenderDrawColor(renderer,
                           border.r,
                           border.g,
                           border.b,
                           border.a);
    SDL_RenderDrawRect(renderer, &dstRect);

    // Pintar hijos usando helper
    component_paint_children(self, renderer);
    return 1;
}

// -------------------------------
// Destroy del Panel
// -------------------------------
void panel_destroy(Panel *panel) {
    if (!panel) return;
    // Usar el destroy del componente base que maneja todo correctamente
    component_destroy(&panel->base);
}

// -------------------------------
// Overrides de color
// -------------------------------
void panel_set_bg_color(Panel *panel, Color bg) {
    if (!panel) return;
    panel->use_custom_bg = 1;
    panel->custom_bg = bg;
}

void panel_set_border_color(Panel *panel, Color border) {
    if (!panel) return;
    panel->use_custom_border = 1;
    panel->custom_border = border;
}

void panel_clear_bg_color(Panel *panel) {
    if (!panel) return;
    panel->use_custom_bg = 0;
}

void panel_clear_border_color(Panel *panel) {
    if (!panel) return;
    panel->use_custom_border = 0;
}
