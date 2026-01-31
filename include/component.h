#ifndef COMPONENT_H
#define COMPONENT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "theme.h"

// -------------------------------
// Tipos auxiliares
// -------------------------------
typedef struct {
    int width;
    int height;
} Size;

typedef struct {
    int x;
    int y;
} Position;


// -------------------------------
// Definición de Component
// -------------------------------
typedef struct Component Component;

typedef void (*EventCallback)(Component *self, SDL_Event *event);

struct Component {
    int id;
    int parentId;
    Size size;
    Position pos;
    int visible;

    int *childIds;
    int childCount;

    SDL_Texture *texture;
    SDL_Rect srcRect;

    char *text;
    TTF_Font *font;

    // Callbacks genéricos
    EventCallback onClick;
    EventCallback onHover;
    EventCallback onPress;
    EventCallback onRelease;

    // Métodos
    void (*init)(Component *self);
    void (*add_child)(Component *self, int childId);
    void (*remove_child)(Component *self, int childId);
    int  (*update)(Component *self, SDL_Event *event); // devuelve 1 si consume evento
    int  (*paint)(Component *self, SDL_Renderer *renderer);
    void (*destroy)(Component *self);

    void (*set_text)(Component *self, const char *txt, TTF_Font *font);
};

// -------------------------------
// Funciones globales
// -------------------------------
int generateId();
Component* findComponentById(int id);
void removeComponentFromGlobalArray(Component *comp);

// Funciones helper para reducir duplicación
int component_propagate_event_to_children(Component *self, SDL_Event *event);
void component_paint_children(Component *self, SDL_Renderer *renderer);
void component_attach_to_parent(Component *child, int parentId);

// Declaración de métodos básicos
void component_init(Component *self);
void component_add_child(Component *self, int childId);
void component_remove_child(Component *self, int childId);
int  component_update(Component *self, SDL_Event *event);
int  component_paint(Component *self, SDL_Renderer *renderer);
void component_destroy(Component *self);

void set_text(Component *self, const char *txt, TTF_Font *font);

#endif // COMPONENT_H
