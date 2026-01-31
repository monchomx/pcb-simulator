// celda.c - Componente de celda del tablero PCB
#include "celda.h"
#include "context.h"
#include <stdlib.h>

// Forward declarations
static void celda_destroy_wrapper(Component *comp);
static int celda_update(Component *comp, SDL_Event *event);

// Inicializar celda (no utilizado actualmente)
static void celda_init(Component *comp, int x, int y, int size) {
    comp->pos.x = x;
    comp->pos.y = y;
    comp->size.width = size;
    comp->size.height = size;
    comp->visible = 1;
}

// Renderizar celda con su textura
static int celda_paint(Component *comp, SDL_Renderer *renderer) {
    Celda *celda = (Celda*)comp;
    
    SDL_Rect dest_rect = {comp->pos.x, comp->pos.y, comp->size.width, comp->size.height};
    
    // Obtener la textura del atlas
    TextureRef *tex_ref = assets_get_texture(celda->texture_id);
    SDL_Texture *atlas = assets_get_atlas_texture();
    
    if (!tex_ref || !atlas) {
        // Fallback a rectángulo blanco si no hay textura
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderFillRect(renderer, &dest_rect);
        return 0;
    }
    
    // Renderizar la textura del atlas
    // Si hay rotación, usar SDL_RenderCopyEx
    if (tex_ref->rotation_degrees != 0) {
        SDL_RenderCopyEx(renderer, atlas, &tex_ref->src_rect, &dest_rect, 
                         tex_ref->rotation_degrees, NULL, SDL_FLIP_NONE);
    } else {
        SDL_RenderCopy(renderer, atlas, &tex_ref->src_rect, &dest_rect);
    }
    
    // Si tiene FLAG_MOUSE_HOVER, dibujar V_HOVER encima
    if (celda->status_flags & FLAG_MOUSE_HOVER) {
        TextureRef *hover_tex = assets_get_texture(TEXTURE_V_HOVER);
        if (hover_tex) {
            SDL_RenderCopy(renderer, atlas, &hover_tex->src_rect, &dest_rect);
        }
    }
    
    return 0;
}

// Actualizar celda (detectar eventos)
static int celda_update(Component *comp, SDL_Event *event) {
    Celda *celda = (Celda*)comp;
    
    if (event->type == SDL_MOUSEMOTION) {
        int mx = event->motion.x;
        int my = event->motion.y;
        
        // Verificar si el mouse está dentro de esta celda
        if (mx >= comp->pos.x && mx < comp->pos.x + comp->size.width &&
            my >= comp->pos.y && my < comp->pos.y + comp->size.height) {
            celda->status_flags |= FLAG_MOUSE_HOVER;
        } else {
            celda->status_flags &= ~FLAG_MOUSE_HOVER;
        }
    }
    
    return 0;
}

// Crear celda
Celda* celda_create(int x, int y, int size) {
    Celda *celda = (Celda*)malloc(sizeof(Celda));
    if (!celda) return NULL;
    
    Component *comp = &celda->base;
    
    // Inicializar como componente (esto registra en el array global)
    component_init(comp);
    
    // Establecer posición y tamaño
    comp->pos.x = x;
    comp->pos.y = y;
    comp->size.width = size;
    comp->size.height = size;
    
    // Sobrescribir paint y destroy con los personalizados
    comp->paint = celda_paint;
    comp->destroy = celda_destroy_wrapper;
    comp->update = celda_update;
    
    // Inicializar datos de celda
    celda->texture_id = TEXTURE_EMPTY;
    celda->status_flags = FLAG_DEFAULT_NONE;
    
    return celda;
}

// Wrapper para destroy que recibe Component*
static void celda_destroy_wrapper(Component *comp) {
    if (!comp) return;
    
    Celda *celda = (Celda*)comp;
    
    // Remover del array global
    removeComponentFromGlobalArray(comp);
    
    // Liberar children
    for (int i = 0; i < comp->childCount; i++) {
        Component *child = findComponentById(comp->childIds[i]);
        if (child && child->destroy) {
            child->destroy(child);
        }
    }
    
    free(celda);
}

// Destruir celda
void celda_destroy(Celda *celda) {
    if (!celda) return;
    celda_destroy_wrapper(&celda->base);
}

// Establecer textura
void celda_set_texture(Celda *celda, TextureID texture_id) {
    if (celda) {
        celda->texture_id = texture_id;
    }
}

// Obtener textura
TextureID celda_get_texture(Celda *celda) {
    return celda ? celda->texture_id : TEXTURE_EMPTY;
}
