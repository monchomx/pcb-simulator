#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "context.h"
#include "theme.h"
#include "assets.h"
#include "gui.h"
#include "app.h"

int main(int argc, char *argv[]) {
    // Inicializar contexto global
    if (!context_init("PCB Simulator", 800, 600)) {
        return -1;
    }

    // Inicializar tema y assets
    theme_init_default();
    assets_load(ctx.renderer);

    // Crear GUI
    if (!gui_create()) {
        context_destroy();
        return -1;
    }

    // Ejecutar aplicación
    app_run();

    // Liberar recursos
    gui_destroy();
    assets_free();
    context_destroy();

    return 0;
}

