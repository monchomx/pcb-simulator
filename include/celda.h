#ifndef CELDA_H
#define CELDA_H

#include "component.h"
#include "assets.h"

// Celda: una celda del tablero PCB
// Cada celda tiene una textura asignada (borde, relleno, etc.)

// Status flags para celda
#define FLAG_DEFAULT_NONE   0x00
#define FLAG_MOUSE_HOVER    0x01

typedef struct {
    Component base;
    TextureID texture_id;    // Textura actualmente mostrada
    int status_flags;        // Flags de estado (hover, selección, etc.)
} Celda;

// Crear una celda
Celda* celda_create(int x, int y, int size);

// Inicializar celda (uso interno/avanzado)
void celda_init(Celda *celda, int x, int y, int size);

// Destruir celda
void celda_destroy(Celda *celda);

// Establecer textura de celda
void celda_set_texture(Celda *celda, TextureID texture_id);

// Obtener textura
TextureID celda_get_texture(Celda *celda);

#endif // CELDA_H
