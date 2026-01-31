#include "button.h"
#include <stdlib.h>
#include <string.h>

// -------------------------------
// Crear un botón
// -------------------------------
Button* button_create(int parentId, Position pos, Size size, const char *txt, TTF_Font *font) {
    Button *btn = (Button*)malloc(sizeof(Button));
    if (!btn) return NULL;

    button_init(btn);
    btn->base.pos = pos;
    btn->base.size = size;
    btn->text = strdup(txt);
    btn->font = font;

    // Colores por defecto
    btn->normalBg  = (Color){0, 120, 200, 255};
    btn->hoverBg   = (Color){0, 150, 250, 255};
    btn->pressedBg = (Color){0, 80, 150, 255};

    btn->normalFg  = (Color){255, 255, 255, 255};
    btn->hoverFg   = (Color){255, 255, 255, 255};
    btn->pressedFg = (Color){200, 200, 200, 255};

    // Usar helper para vincular al padre
    component_attach_to_parent(&btn->base, parentId);

    return btn;
}

// -------------------------------
// Inicializar botón
// -------------------------------
void button_init(Button *btn) {
    component_init(&btn->base);

    btn->state = BTN_NORMAL;
    btn->onClick = NULL;

    // Sobrescribir punteros de función
    btn->base.update  = button_update;
    btn->base.paint   = button_paint;
    btn->base.destroy = (void (*)(Component*))button_destroy;
}

// -------------------------------
// Update del botón
// -------------------------------
int button_update(Component *self, SDL_Event *event) {
    Button *btn = (Button*)self;
    SDL_Rect rect = { self->pos.x, self->pos.y, self->size.width, self->size.height };

    if (event->type == SDL_MOUSEMOTION) {
        if (SDL_PointInRect(&(SDL_Point){event->motion.x, event->motion.y}, &rect)) {
            btn->state = BTN_HOVER;
        } else {
            btn->state = BTN_NORMAL;
        }
    }
    else if (event->type == SDL_MOUSEBUTTONDOWN) {
        if (SDL_PointInRect(&(SDL_Point){event->button.x, event->button.y}, &rect)) {
            btn->state = BTN_PRESSED;
        }
    }
    else if (event->type == SDL_MOUSEBUTTONUP) {
        if (SDL_PointInRect(&(SDL_Point){event->button.x, event->button.y}, &rect)) {
            btn->state = BTN_HOVER;
            if (btn->onClick) btn->onClick(btn, event);
            return 1; // consumir evento
        }
    }

    // Propagar a hijos usando helper
    return component_propagate_event_to_children(self, event);
}

// -------------------------------
// Paint del botón
// -------------------------------
int button_paint(Component *self, SDL_Renderer *renderer) {
    if (!self->visible) return 0;
    
    Button *btn = (Button*)self;
    SDL_Rect dstRect = { self->pos.x, self->pos.y, self->size.width, self->size.height };

    Color bg, fg, border;
    switch (btn->state) {
        case BTN_NORMAL:
            bg = currentTheme.button_normal_bg;
            fg = currentTheme.button_normal_fg;
            border = currentTheme.button_normal_border;
            break;
        case BTN_HOVER:
            bg = currentTheme.button_hover_bg;
            fg = currentTheme.button_hover_fg;
            border = currentTheme.button_hover_border;
            break;
        case BTN_PRESSED:
            bg = currentTheme.button_pressed_bg;
            fg = currentTheme.button_pressed_fg;
            border = currentTheme.button_pressed_border;
            break;
    }

    // Fondo
    SDL_SetRenderDrawColor(renderer, bg.r, bg.g, bg.b, bg.a);
    SDL_RenderFillRect(renderer, &dstRect);

    // Texto
    if (btn->text && btn->font) {
        SDL_Color sdlFg = { fg.r, fg.g, fg.b, fg.a };
        SDL_Surface *textSurface = TTF_RenderUTF8_Blended(btn->font, btn->text, sdlFg);
        if (textSurface) {
            SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            if (textTexture) {
                SDL_Rect textRect = { self->pos.x + 10, self->pos.y + 10, textSurface->w, textSurface->h };
                SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
                SDL_DestroyTexture(textTexture);
            }
            SDL_FreeSurface(textSurface);
        }
    }

    // Borde
    SDL_SetRenderDrawColor(renderer, border.r, border.g, border.b, border.a);
    SDL_RenderDrawRect(renderer, &dstRect);

    // Pintar hijos usando helper
    component_paint_children(self, renderer);
    return 1;
}

// -------------------------------
// Destroy del botón
// -------------------------------
void button_destroy(Button *btn) {
    if (!btn) return;
    // Usar el destroy del componente base que maneja todo correctamente
    component_destroy(&btn->base);
}
