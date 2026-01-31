#ifndef APP_H
#define APP_H

// Configuración de la aplicación
#define TARGET_FPS 120
#define FRAME_TIME (1000 / TARGET_FPS)  // 8.33ms

// Funciones principales
void app_run();
int app_process_frame();
void app_delay_frame();

#endif // APP_H
