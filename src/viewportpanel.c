#include "viewportpanel.h"
#include <SDL2/SDL.h>
#include <stdlib.h>
#include <math.h>

static float viewportpanel_get_scale(const ViewportPanel *vp);
static void viewportpanel_apply_inner_position(const ViewportPanel *vp, Component *self, int *oldX, int *oldY);
static void viewportpanel_restore_inner_position(ViewportPanel *vp, int oldX, int oldY);
static int viewportpanel_dispatch_to_inner(ViewportPanel *vp, Component *self, SDL_Event *event);

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

    vp->zoomLevel = 3; // Zoom default
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
            // Zoom centrado en el mouse
            int mouse_x = 0, mouse_y = 0;
            SDL_GetMouseState(&mouse_x, &mouse_y);

            float oldScale = viewportpanel_get_scale(vp);

            int newZoom = vp->zoomLevel;
            if (event->wheel.y > 0) newZoom++;
            else if (event->wheel.y < 0 && vp->zoomLevel > 1) newZoom--;

            vp->zoomLevel = newZoom;
            float newScale = viewportpanel_get_scale(vp);

            // Coordenadas del mundo bajo el mouse antes del zoom
            float world_x = (mouse_x / oldScale) - self->pos.x - vp->offsetX;
            float world_y = (mouse_y / oldScale) - self->pos.y - vp->offsetY;

            // Ajustar offsets para mantener el punto bajo el mouse
            vp->offsetX = (int)lroundf((mouse_x / newScale) - self->pos.x - world_x);
            vp->offsetY = (int)lroundf((mouse_y / newScale) - self->pos.y - world_y);
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
        // Forzar actualización de hover con la posición actual del mouse
        int mx = 0, my = 0;
        SDL_GetMouseState(&mx, &my);
        SDL_Event hoverEvent;
        hoverEvent.type = SDL_MOUSEMOTION;
        hoverEvent.motion.x = mx;
        hoverEvent.motion.y = my;
        (void)viewportpanel_dispatch_to_inner(vp, self, &hoverEvent);

        return 1; // consumir evento
    }

    // Para eventos de mouse, ajustar por zoom y propagar al inner con su posición efectiva
    if (event->type == SDL_MOUSEMOTION || event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEBUTTONUP) {
        return viewportpanel_dispatch_to_inner(vp, self, event);
    }

    // Propagar eventos usando helper
    return component_propagate_event_to_children(self, event);
}

static float viewportpanel_get_scale(const ViewportPanel *vp) {
    if (!vp || vp->zoomLevel <= 0) return 1.0f;
    return vp->zoomLevel / 3.0f; // Nivel 3 = 100%
}

static void viewportpanel_apply_inner_position(const ViewportPanel *vp, Component *self, int *oldX, int *oldY) {
    if (!vp || !vp->inner) return;
    *oldX = vp->inner->base.pos.x;
    *oldY = vp->inner->base.pos.y;
    vp->inner->base.pos.x = self->pos.x + vp->offsetX;
    vp->inner->base.pos.y = self->pos.y + vp->offsetY;
}

static void viewportpanel_restore_inner_position(ViewportPanel *vp, int oldX, int oldY) {
    if (!vp || !vp->inner) return;
    vp->inner->base.pos.x = oldX;
    vp->inner->base.pos.y = oldY;
}

static int viewportpanel_dispatch_to_inner(ViewportPanel *vp, Component *self, SDL_Event *event) {
    float z = viewportpanel_get_scale(vp);
    int oldX = 0;
    int oldY = 0;

    if (event->type == SDL_MOUSEMOTION) {
        int orig_x = event->motion.x;
        int orig_y = event->motion.y;

        event->motion.x = (int)(orig_x / z);
        event->motion.y = (int)(orig_y / z);

        viewportpanel_apply_inner_position(vp, self, &oldX, &oldY);
        int result = (vp->inner && vp->inner->base.update)
                         ? vp->inner->base.update((Component*)vp->inner, event)
                         : 0;
        viewportpanel_restore_inner_position(vp, oldX, oldY);

        event->motion.x = orig_x;
        event->motion.y = orig_y;
        return result;
    }

    if (event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEBUTTONUP) {
        int orig_x = event->button.x;
        int orig_y = event->button.y;

        event->button.x = (int)(orig_x / z);
        event->button.y = (int)(orig_y / z);

        viewportpanel_apply_inner_position(vp, self, &oldX, &oldY);
        int result = (vp->inner && vp->inner->base.update)
                         ? vp->inner->base.update((Component*)vp->inner, event)
                         : 0;
        viewportpanel_restore_inner_position(vp, oldX, oldY);

        event->button.x = orig_x;
        event->button.y = orig_y;
        return result;
    }

    return 0;
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
        // Aplicar zoom (nivel 3 = 100%)
        float zoomScale = viewportpanel_get_scale(vp);
        SDL_RenderSetScale(renderer, zoomScale, zoomScale);

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
