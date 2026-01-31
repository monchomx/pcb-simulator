#ifndef VIEWPORTPANEL_H
#define VIEWPORTPANEL_H

#include "panel.h"

typedef struct ViewportPanel {
    Panel base;          // hereda de Panel
    Panel *inner;        // panel interno grande
    int zoomLevel;       // entero: 1x, 2x, 3x...
    int offsetX, offsetY;// desplazamiento actual
} ViewportPanel;

ViewportPanel* viewportpanel_create(int parentId, Position pos, Size size, Panel *inner);
void viewportpanel_init(ViewportPanel *panel);
int viewportpanel_update(Component *self, SDL_Event *event);
int viewportpanel_paint(Component *self, SDL_Renderer *renderer);
void viewportpanel_destroy(Component *self);

#endif
