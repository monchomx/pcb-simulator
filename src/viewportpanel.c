#include "viewportpanel.h"
#include <SDL2/SDL.h>
#include <stdlib.h>

ViewportPanel* viewportpanel_create(int parentId, Position pos, Size size, Panel *inner) {
    ViewportPanel *vp = (ViewportPanel*)malloc(sizeof(ViewportPanel));
    if (!vp) return NULL;
    
    viewportpanel_init(vp);
    vp->base.base.pos = pos;
    vp->base.base.size = size;
    vp->inner = inner;

    // Añadir el panel inner como hijo del viewport
    if (inner) {
        component_add_child((Component*)vp, inner->base.id);
        inner->base.parentId = vp->base.base.id;
    }

    // Usar helper para vincular al padre
    component_attach_to_parent((Component*)vp, parentId);

    return vp;
}

void viewportpanel_init(ViewportPanel *vp) {
    component_init((Component*)vp);

    vp->zoomLevel = 1;
    vp->offsetX = 0;
    vp->offsetY = 0;

    // Sobrescribir punteros de función
    vp->base.base.update  = viewportpanel_update;
    vp->base.base.paint   = viewportpanel_paint;
    vp->base.base.destroy = viewportpanel_destroy;
}

int viewportpanel_update(Component *self, SDL_Event *event) {
    ViewportPanel *vp = (ViewportPanel*)self;
    if (event->type == SDL_MOUSEWHEEL) {
        const Uint8 *keys = SDL_GetKeyboardState(NULL);

        if (keys[SDL_SCANCODE_LCTRL] || keys[SDL_SCANCODE_RCTRL]) {
            // Zoom
            if (event->wheel.y > 0) vp->zoomLevel++;
            else if (event->wheel.y < 0 && vp->zoomLevel > 1) vp->zoomLevel--;
        }
        else if (keys[SDL_SCANCODE_LSHIFT] || keys[SDL_SCANCODE_RSHIFT]) {
            // Desplazamiento horizontal
            if (event->wheel.y > 0) vp->offsetX += 20;
            else if (event->wheel.y < 0) vp->offsetX -= 20;
        }
        else {
            // Desplazamiento vertical
            if (event->wheel.y > 0) vp->offsetY += 20;
            else if (event->wheel.y < 0) vp->offsetY -= 20;
        }
        return 1; // consumir evento
    }

    // Propagar eventos usando helper
    return component_propagate_event_to_children(self, event);
}

int viewportpanel_paint(Component *self, SDL_Renderer *renderer) {
    ViewportPanel *vp = (ViewportPanel*)self;

    if (!vp->inner) {
        return 0;
    }

    // Definir área visible (rectángulo del viewport)
    SDL_Rect clip = { self->pos.x, self->pos.y, self->size.width, self->size.height };
    SDL_RenderSetClipRect(renderer, &clip);

    // Fondo visible del viewport (más claro que panel)
    SDL_SetRenderDrawColor(renderer,
                           currentTheme.viewport_bg.r,
                           currentTheme.viewport_bg.g,
                           currentTheme.viewport_bg.b,
                           currentTheme.viewport_bg.a);
    SDL_RenderFillRect(renderer, &clip);

    // Guardar posición original del inner
    int oldX = vp->inner->base.pos.x;
    int oldY = vp->inner->base.pos.y;

    // Aplicar offset
    vp->inner->base.pos.x = self->pos.x + vp->offsetX;
    vp->inner->base.pos.y = self->pos.y + vp->offsetY;

    // Aplicar zoom
    SDL_RenderSetScale(renderer, vp->zoomLevel, vp->zoomLevel);

    // Pintar el panel interno
    int consumed = 0;
    Component *innerComp = (Component*)vp->inner;
    if (innerComp->paint) {
        consumed = innerComp->paint(innerComp, renderer);
    }

    // Restaurar estado
    SDL_RenderSetScale(renderer, 1.0, 1.0);
    SDL_RenderSetClipRect(renderer, NULL);

    // Restaurar posición original
    vp->inner->base.pos.x = oldX;
    vp->inner->base.pos.y = oldY;

    return consumed; // devolver si el evento fue consumido
}


void viewportpanel_destroy(Component *self) {
    ViewportPanel *vp = (ViewportPanel*)self;
    // Destruir hijos primero (incluye inner)
    for (int i = 0; i < self->childCount; i++) {
        Component *child = findComponentById(self->childIds[i]);
        if (child && child->destroy) {
            child->destroy(child);
        }
    }
    // Remover del array global
    removeComponentFromGlobalArray(self);
    // Liberar recursos
    if (self->childIds) free(self->childIds);
    if (self->text) free(self->text);
    free(vp);
}
