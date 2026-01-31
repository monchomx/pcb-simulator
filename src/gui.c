#include "gui.h"
#include "pcb.h"
#include "celda.h"
#include "context.h"
#include "theme.h"
#include <stdio.h>

// Variable global de GUI
static GUI g_gui = {0};

// -----------------------------------------------
// Inicializar GUI
// -----------------------------------------------
GUI* gui_create() {
    // Panel raíz
    g_gui.rootPanel = panel_create(0, (Position){0, 0}, (Size){ctx.width, ctx.height});
    if (!g_gui.rootPanel) return NULL;

    // Panel canvas interno grande (ej. canvas de 1000x1000)
    // IMPORTANTE: Sin padre (0) porque será hijo del viewport
    g_gui.canvas = panel_create(0, (Position){0, 0}, (Size){1000, 1000});
    if (!g_gui.canvas) {
        g_gui.rootPanel->base.destroy((Component*)g_gui.rootPanel);
        return NULL;
    }

    // Personalizar colores del canvas
    panel_set_bg_color(g_gui.canvas, (Color){255, 255, 255, 255});
    panel_set_border_color(g_gui.canvas, (Color){220, 220, 220, 255});

    // Crear PCB dentro del canvas (20x10 celdas de 11x11)
    // Por defecto todas las celdas inician con TEXTURE_EMPTY
    PCB *pcb = pcb_create(g_gui.canvas->base.id, (Position){10, 10}, 20, 10);

    // Crear viewport que muestra el canvas
    g_gui.viewport = viewportpanel_create(g_gui.rootPanel->base.id,
                                          (Position){50, 50},
                                          (Size){600, 500},
                                          g_gui.canvas);
    if (!g_gui.viewport) {
        g_gui.rootPanel->base.destroy((Component*)g_gui.rootPanel);
        return NULL;
    }

    return &g_gui;
}

// -----------------------------------------------
// Procesar eventos
// -----------------------------------------------
void gui_update(SDL_Event *event) {
    if (!g_gui.rootPanel) return;

    // Propagar evento desde el rootPanel hacia toda la jerarquía de componentes
    // El sistema de propagación detiene si algún componente consume el evento
    g_gui.rootPanel->base.update((Component*)g_gui.rootPanel, event);
}

// -----------------------------------------------
// Actualizar lógica (sin eventos)
// -----------------------------------------------
void gui_logic_update() {
    if (!g_gui.rootPanel) return;

    // Aquí irán actualizaciones de lógica general:
    // - Animaciones
    // - Cálculos
    // - Estados que no dependen de eventos
    // Por ahora está vacío, pero es el lugar para agregar lógica
}

// -----------------------------------------------
// Pintar GUI
// -----------------------------------------------
void gui_paint() {
    if (!g_gui.rootPanel) return;

    // Limpiar pantalla
    SDL_SetRenderDrawColor(ctx.renderer,
                           currentTheme.background.r,
                           currentTheme.background.g,
                           currentTheme.background.b,
                           currentTheme.background.a);
    SDL_RenderClear(ctx.renderer);

    // Pintar el rootPanel (que contiene viewport y demás)
    component_paint((Component*)g_gui.rootPanel, ctx.renderer);

    // Presentar
    SDL_RenderPresent(ctx.renderer);
}

// -----------------------------------------------
// Destruir GUI
// -----------------------------------------------
void gui_destroy() {
    if (g_gui.rootPanel) {
        g_gui.rootPanel->base.destroy((Component*)g_gui.rootPanel);
        g_gui.rootPanel = NULL;
    }
}
