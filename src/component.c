#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "component.h"

// -------------------------------
// Variables globales
// -------------------------------
#define MAX_COMPONENTS 1000 // Límite más realista
static int globalIdCounter = 1;
static Component* components[MAX_COMPONENTS];
static int componentCount = 0;

// -------------------------------
// Funciones globales
// -------------------------------
int generateId() {
    return globalIdCounter++;
}

Component* findComponentById(int id) {
    for (int i = 0; i < componentCount; i++) {
        if (components[i]->id == id) return components[i];
    }
    return NULL;
}

void removeComponentFromGlobalArray(Component *comp) {
    for (int i = 0; i < componentCount; i++) {
        if (components[i] == comp) {
            for (int j = i; j < componentCount - 1; j++) {
                components[j] = components[j + 1];
            }
            componentCount--;
            break;
        }
    }
}

// -------------------------------
// Funciones helper para reducir duplicación
// -------------------------------

// Propagar eventos a todos los hijos
int component_propagate_event_to_children(Component *self, SDL_Event *event) {
    for (int i = 0; i < self->childCount; i++) {
        Component *child = findComponentById(self->childIds[i]);
        if (child && child->update) {
            int consumed = child->update(child, event);
            if (consumed) return 1;
        }
    }
    return 0;
}

// Pintar hijos con offset relativo al padre
void component_paint_children(Component *self, SDL_Renderer *renderer) {
    for (int i = 0; i < self->childCount; i++) {
        Component *child = findComponentById(self->childIds[i]);
        if (child && child->paint) {
            // Guardar posición original
            int origX = child->pos.x;
            int origY = child->pos.y;
            
            // Aplicar offset del padre
            child->pos.x += self->pos.x;
            child->pos.y += self->pos.y;
            
            // Pintar
            child->paint(child, renderer);
            
            // Restaurar posición original
            child->pos.x = origX;
            child->pos.y = origY;
        }
    }
}

// Vincular hijo a padre (lógica común de creación)
void component_attach_to_parent(Component *child, int parentId) {
    if (parentId != 0) {
        Component *parent = findComponentById(parentId);
        if (parent && parent->add_child) {
            parent->add_child(parent, child->id);
            child->parentId = parentId;
        }
    }
}

// -------------------------------
// Métodos básicos de Component
// -------------------------------
void component_init(Component *self) {
    self->id = generateId();
    self->parentId = 0;
    self->size = (Size){0,0};
    self->pos = (Position){0,0};
    self->visible = 1;

    self->childIds = NULL;
    self->childCount = 0;

    self->texture = NULL;
    self->srcRect = (SDL_Rect){0,0,0,0};

    self->text = NULL;
    self->font = NULL;

    self->onClick = NULL;
    self->onHover = NULL;
    self->onPress = NULL;
    self->onRelease = NULL;

    // Asignar punteros a funciones
    self->init = component_init;
    self->add_child = component_add_child;
    self->remove_child = component_remove_child;
    self->update = component_update;
    self->paint = component_paint;
    self->destroy = component_destroy;
    self->set_text = set_text;

    // Registrar en el mapa global
    if (componentCount < MAX_COMPONENTS) {
        components[componentCount++] = self;
    }
}

void component_add_child(Component *self, int childId) {
    self->childIds = realloc(self->childIds, sizeof(int) * (self->childCount + 1));
    self->childIds[self->childCount++] = childId;
}

void component_remove_child(Component *self, int childId) {
    for (int i = 0; i < self->childCount; i++) {
        if (self->childIds[i] == childId) {
            for (int j = i; j < self->childCount - 1; j++) {
                self->childIds[j] = self->childIds[j+1];
            }
            self->childCount--;
            self->childIds = realloc(self->childIds, sizeof(int) * self->childCount);
            break;
        }
    }
}

// -------------------------------
// Update con propagación
// -------------------------------
int component_update(Component *self, SDL_Event *event) {
    SDL_Rect rect = { self->pos.x, self->pos.y, self->size.width, self->size.height };

    if (event->type == SDL_MOUSEMOTION) {
        if (SDL_PointInRect(&(SDL_Point){event->motion.x, event->motion.y}, &rect)) {
            if (self->onHover) self->onHover(self, event);
        }
    }
    else if (event->type == SDL_MOUSEBUTTONDOWN) {
        if (SDL_PointInRect(&(SDL_Point){event->button.x, event->button.y}, &rect)) {
            if (self->onPress) self->onPress(self, event);
        }
    }
    else if (event->type == SDL_MOUSEBUTTONUP) {
        if (SDL_PointInRect(&(SDL_Point){event->button.x, event->button.y}, &rect)) {
            if (self->onClick) self->onClick(self, event);
            if (self->onRelease) self->onRelease(self, event);
            return 1; // consumir evento → detener propagación
        }
    }

    // Propagar a hijos usando helper
    return component_propagate_event_to_children(self, event);
}

// -------------------------------
// Paint recursivo
// -------------------------------
int component_paint(Component *self, SDL_Renderer *renderer) {
    if (!self->visible) return 0;

    SDL_Rect dstRect = { self->pos.x, self->pos.y, self->size.width, self->size.height };

    // Fondo
    SDL_SetRenderDrawColor(renderer,
                           currentTheme.background.r,
                           currentTheme.background.g,
                           currentTheme.background.b,
                           currentTheme.background.a);
    SDL_RenderFillRect(renderer, &dstRect);

    // Textura opcional
    if (self->texture) {
        SDL_RenderCopy(renderer, self->texture, &self->srcRect, &dstRect);
    }

    // Texto opcional
    if (self->text && self->font) {
        SDL_Color fg = { currentTheme.text_primary.r,
                         currentTheme.text_primary.g,
                         currentTheme.text_primary.b,
                         currentTheme.text_primary.a};
        SDL_Surface *textSurface = TTF_RenderText_Solid(self->font, self->text, fg);
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
                           currentTheme.panel_border.r,
                           currentTheme.panel_border.g,
                           currentTheme.panel_border.b,
                           currentTheme.panel_border.a);
    SDL_RenderDrawRect(renderer, &dstRect);

    // Pintar hijos usando helper
    component_paint_children(self, renderer);
    return 1;
}

void component_destroy(Component *self) {
    if (!self) return;

    // Primero destruir hijos recursivamente
    for (int i = 0; i < self->childCount; i++) {
        Component *child = findComponentById(self->childIds[i]);
        if (child && child->destroy) {
            child->destroy(child); // cada hijo se destruye a sí mismo
        }
    }

    // Remover del array global de componentes
    removeComponentFromGlobalArray(self);

    // Liberar recursos propios
    if (self->text) free(self->text);
    if (self->childIds) free(self->childIds);

    // Finalmente liberar el propio componente
    free(self);
}

// -------------------------------
// Setters
// -------------------------------

void set_text(Component *self, const char *txt, TTF_Font *font) {
    if (self->text) free(self->text);
    self->text = strdup(txt);
    self->font = font;
}
