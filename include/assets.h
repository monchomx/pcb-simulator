// assets.h
#ifndef ASSETS_H
#define ASSETS_H

#include <SDL2/SDL.h>

// -----------------------------------------------
// Enumeración de texturas del atlas PCB
// -----------------------------------------------
typedef enum {
    // Base
    TEXTURE_EMPTY = 0,
    
    // Borde - Líneas (4 rotaciones)
    TEXTURE_B_LINE_R = 1,
    TEXTURE_B_LINE_U = 2,
    TEXTURE_B_LINE_L = 3,
    TEXTURE_B_LINE_D = 4,
    
    // Borde - Diagonales (4 rotaciones)
    TEXTURE_B_DIAG_RU = 5,
    TEXTURE_B_DIAG_LU = 6,
    TEXTURE_B_DIAG_LD = 7,
    TEXTURE_B_DIAG_RD = 8,
    
    // Borde - Vía
    TEXTURE_B_VIA = 9,
    
    // Relleno - Líneas (4 rotaciones)
    TEXTURE_F_LINE_R = 10,
    TEXTURE_F_LINE_U = 11,
    TEXTURE_F_LINE_L = 12,
    TEXTURE_F_LINE_D = 13,
    
    // Relleno - Diagonales (4 rotaciones)
    TEXTURE_F_DIAG_RU = 14,
    TEXTURE_F_DIAG_LU = 15,
    TEXTURE_F_DIAG_LD = 16,
    TEXTURE_F_DIAG_RD = 17,
    
    // Relleno - Vía
    TEXTURE_F_VIA = 18,
    
    // Visual - Corners (4 rotaciones)
    TEXTURE_V_CORNER_RU = 19,
    TEXTURE_V_CORNER_LU = 20,
    TEXTURE_V_CORNER_LD = 21,
    TEXTURE_V_CORNER_RD = 22,
    
    // Visual - Hover
    TEXTURE_V_HOVER = 23,
    
    TEXTURE_COUNT = 24
} TextureID;

// -----------------------------------------------
// Referencia a textura en el atlas
// -----------------------------------------------
typedef struct {
    SDL_Rect src_rect;      // Rectángulo en el atlas
    int base_atlas_index;   // Índice base en el atlas (pre-rotación)
    int rotation_degrees;   // Rotación aplicada (0, 90, 180, 270)
} TextureRef;

// -----------------------------------------------
// Funciones públicas
// -----------------------------------------------
void assets_load(SDL_Renderer *renderer);
void assets_free();

// Obtener referencia a textura por ID
TextureRef* assets_get_texture(TextureID id);

// Obtener el atlas SDL_Texture
SDL_Texture* assets_get_atlas_texture();

#endif
