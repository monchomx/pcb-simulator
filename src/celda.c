// celda.c - Componente de celda del tablero PCB
#include "celda.h"
#include "context.h"
#include <stdlib.h>

// Inicializar celda
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
    
    // Obtener referencia a textura
    TextureRef *tex_ref = assets_get_texture(celda->texture_id);
    if (!tex_ref) return 0;
    
    // Obtener atlas
    SDL_Texture *atlas = assets_get_atlas_texture();
    if (!atlas) return 0;
    
    // Rectángulo de destino en pantalla
    SDL_Rect dest_rect = {comp->pos.x, comp->pos.y, comp->size.width, comp->size.height};
    
    // Renderizar con rotación si es necesaria
    if (tex_ref->rotation_degrees == 0) {
        SDL_RenderCopy(renderer, atlas, &tex_ref->src_rect, &dest_rect);
    } else {
        SDL_RenderCopyEx(renderer, atlas, &tex_ref->src_rect, &dest_rect,
                        tex_ref->rotation_degrees, NULL, SDL_FLIP_NONE);
    }
    
    // Renderizar children (si hay)
    component_paint_children(comp, renderer);
    
    return 0;
}

// Crear celda
Celda* celda_create(int x, int y, int size) {
    Celda *celda = (Celda*)malloc(sizeof(Celda));
    if (!celda) return NULL;
    
    // Inicializar como componente
    Component *comp = &celda->base;
    comp->parentId = 0;
    comp->childCount = 0;
    comp->texture = NULL;
    comp->text = NULL;
    comp->font = NULL;
    comp->onClick = NULL;
    comp->onHover = NULL;
    comp->onPress = NULL;
    comp->onRelease = NULL;
    
    // Establecer métodos
    comp->update = NULL;
    comp->paint = celda_paint;
    comp->destroy = (void(*)(Component*))celda_destroy;
    
    // Inicializar datos de celda
    celda_init(comp, x, y, size);
    celda->texture_id = TEXTURE_EMPTY;
    celda->direction_flags = 0;
    
    // Registrar en array global
    comp->id = generateId();
    
    return celda;
}

// Destruir celda
void celda_destroy(Celda *celda) {
    if (!celda) return;
    
    Component *comp = &celda->base;
    
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
