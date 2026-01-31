#include "label.h"
#include <stdlib.h>
#include <string.h>

// -------------------------------
// Crear un Label
// -------------------------------
Label* label_create(int parentId, Position pos, Size size, const char *txt, TTF_Font *font) {
    Label *lbl = (Label*)malloc(sizeof(Label));
    if (!lbl) return NULL;

    label_init(lbl);
    lbl->base.pos = pos;
    lbl->base.size = size;
    lbl->text = strdup(txt);
    lbl->font = font;

    // Usar helper para vincular al padre
    component_attach_to_parent(&lbl->base, parentId);

    return lbl;
}

// -------------------------------
// Inicializar Label
// -------------------------------
void label_init(Label *lbl) {
    component_init(&lbl->base);

    lbl->text = NULL;
    lbl->font = NULL;

    // Sobrescribir punteros de función
    lbl->base.update  = label_update;
    lbl->base.paint   = label_paint;
    lbl->base.destroy = (void (*)(Component*))label_destroy;
}

// -------------------------------
// Update del Label
// -------------------------------
int label_update(Component *self, SDL_Event *event) {
    // El Label no consume eventos, solo propaga a hijos
    return component_propagate_event_to_children(self, event);
}

// -------------------------------
// Paint del Label
// -------------------------------
int label_paint(Component *self, SDL_Renderer *renderer) {
    if (!self->visible) return 0;
    Label *lbl = (Label*)self;

    SDL_Rect dstRect = { self->pos.x, self->pos.y, self->size.width, self->size.height };

    // Fondo
    SDL_SetRenderDrawColor(renderer,
                          currentTheme.label_bg.r,
                           currentTheme.label_bg.g,
                           currentTheme.label_bg.b,
                           currentTheme.label_bg.a);
    SDL_RenderFillRect(renderer, &dstRect);

    // Texto
    if (lbl->text && lbl->font) {
        SDL_Color fg = { currentTheme.label_text.r,
                         currentTheme.label_text.g,
                         currentTheme.label_text.b,
                         currentTheme.label_text.a };
        SDL_Surface *textSurface = TTF_RenderUTF8_Blended(lbl->font, lbl->text, fg);
        if (textSurface) {
            SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            if (textTexture) {
                SDL_Rect textRect = { self->pos.x + 5, self->pos.y + 5, textSurface->w, textSurface->h };
                SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
                SDL_DestroyTexture(textTexture);
            }
            SDL_FreeSurface(textSurface);
        }
    }

    // Borde
    SDL_SetRenderDrawColor(renderer,
                           currentTheme.label_border.r,
                           currentTheme.label_border.g,
                           currentTheme.label_border.b,
                           currentTheme.label_border.a);
    SDL_RenderDrawRect(renderer, &dstRect);

    // Pintar hijos usando helper
    component_paint_children(self, renderer);
    return 1;
}

// -------------------------------
// Destroy del Label
// -------------------------------
void label_destroy(Label *lbl) {
    if (!lbl) return;
    // Usar el destroy del componente base que maneja todo correctamente
    component_destroy(&lbl->base);
}
