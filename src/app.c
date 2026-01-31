#include "app.h"
#include "context.h"
#include "gui.h"
#include <SDL2/SDL.h>

// Variables de timing
static uint32_t frame_start_time = 0;
static uint32_t frame_count = 0;
static uint32_t fps_last_time = 0;
static int current_fps = 0;

// -----------------------------------------------
// Loop principal de la aplicación
// -----------------------------------------------
void app_run() {
    fps_last_time = SDL_GetTicks();
    
    while (app_process_frame()) {
        app_delay_frame();
    }
}

// -----------------------------------------------
// Procesar un frame completo
// -----------------------------------------------
int app_process_frame() {
    frame_start_time = SDL_GetTicks();
    
    // 1. Procesar eventos
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            ctx.running = 0;
        }

        // Propagar evento a GUI
        gui_update(&event);
    }
    
    // 2. Actualizar lógica
    gui_logic_update();
    
    // 3. Renderizar
    gui_paint();
    
    // 4. Calcular FPS
    frame_count++;
    uint32_t current_time = SDL_GetTicks();
    if (current_time - fps_last_time >= 1000) {
        current_fps = frame_count;
        frame_count = 0;
        fps_last_time = current_time;
        
        // Opcional: Imprimir FPS en consola (puedes comentar esto)
        // printf("FPS: %d\n", current_fps);
    }
    
    return ctx.running;
}

// -----------------------------------------------
// Delay para mantener FPS constante
// -----------------------------------------------
void app_delay_frame() {
    uint32_t frame_end_time = SDL_GetTicks();
    int frame_time = frame_end_time - frame_start_time;
    
    // Si el frame tomó menos tiempo del target, hacer delay
    if (frame_time < FRAME_TIME) {
        SDL_Delay(FRAME_TIME - frame_time);
    }
    // Si el frame tomó más tiempo, se saltea (no podemos hacer nada)
}
