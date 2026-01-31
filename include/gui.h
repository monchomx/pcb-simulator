#ifndef GUI_H
#define GUI_H

#include "component.h"
#include "panel.h"
#include "viewportpanel.h"

// Estructura principal de GUI
typedef struct {
    Panel *rootPanel;
    ViewportPanel *viewport;
    Panel *canvas;
} GUI;

// Funciones públicas
GUI* gui_create();
void gui_update(SDL_Event *event);
void gui_logic_update();
void gui_paint();
void gui_destroy();

#endif // GUI_H
