#ifndef CELDA_H
#define CELDA_H

#include "component.h"
#include "assets.h"

// Celda: una celda del tablero PCB
// Cada celda tiene una textura asignada (borde, relleno, etc.)

typedef struct {
    Component base;
    TextureID texture_id;    // Textura actualmente mostrada
    int direction_flags;     // Flags de dirección (para trace drawing)
} Celda;

// Crear una celda
Celda* celda_create(int x, int y, int size);

// Destruir celda
void celda_destroy(Celda *celda);

// Establecer textura de celda
void celda_set_texture(Celda *celda, TextureID texture_id);

// Obtener textura
TextureID celda_get_texture(Celda *celda);

#endif // CELDA_H
