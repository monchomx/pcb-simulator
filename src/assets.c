// assets.c
#include "assets.h"
#include "context.h"
#include <SDL2/SDL_image.h>
#include <string.h>
#include <stdio.h>

// -----------------------------------------------
// Variables globales
// -----------------------------------------------
static SDL_Texture *g_atlas_texture = NULL;
static TextureRef g_textures[TEXTURE_COUNT] = {0};

// Constantes
#define TEXTURE_SIZE 11
#define ATLAS_WIDTH 99
#define ATLAS_HEIGHT 11

// -----------------------------------------------
// Mapeo: textura original → índice en el atlas
// -----------------------------------------------
// Texturas originales en el atlas (sin rotaciones):
// Índice 0: EMPTY (11x11 en x=0)
// Índice 1: B_LINE_R (11x11 en x=11)
// Índice 2: B_DIAG_RU (11x11 en x=22)
// Índice 3: B_VIA (11x11 en x=33)
// Índice 4: F_LINE_R (11x11 en x=44)
// Índice 5: F_DIAG_RU (11x11 en x=55)
// Índice 6: F_VIA (11x11 en x=66)
// Índice 7: V_CORNER_RU (11x11 en x=77)
// Índice 8: V_HOVER (11x11 en x=88)

// -----------------------------------------------
// Inicializar tabla de texturas
// -----------------------------------------------
static void assets_init_texture_table() {
    // EMPTY
    g_textures[TEXTURE_EMPTY].src_rect = (SDL_Rect){0, 0, TEXTURE_SIZE, TEXTURE_SIZE};
    g_textures[TEXTURE_EMPTY].base_atlas_index = 0;
    g_textures[TEXTURE_EMPTY].rotation_degrees = 0;
    
    // Borde - Líneas (B_LINE_R con rotaciones)
    // 0°: RIGHT, 90°: UP, 180°: LEFT, 270°: DOWN
    g_textures[TEXTURE_B_LINE_R].src_rect = (SDL_Rect){11, 0, TEXTURE_SIZE, TEXTURE_SIZE};
    g_textures[TEXTURE_B_LINE_R].base_atlas_index = 1;
    g_textures[TEXTURE_B_LINE_R].rotation_degrees = 0;
    
    g_textures[TEXTURE_B_LINE_U].src_rect = (SDL_Rect){11, 0, TEXTURE_SIZE, TEXTURE_SIZE};
    g_textures[TEXTURE_B_LINE_U].base_atlas_index = 1;
    g_textures[TEXTURE_B_LINE_U].rotation_degrees = 90;
    
    g_textures[TEXTURE_B_LINE_L].src_rect = (SDL_Rect){11, 0, TEXTURE_SIZE, TEXTURE_SIZE};
    g_textures[TEXTURE_B_LINE_L].base_atlas_index = 1;
    g_textures[TEXTURE_B_LINE_L].rotation_degrees = 180;
    
    g_textures[TEXTURE_B_LINE_D].src_rect = (SDL_Rect){11, 0, TEXTURE_SIZE, TEXTURE_SIZE};
    g_textures[TEXTURE_B_LINE_D].base_atlas_index = 1;
    g_textures[TEXTURE_B_LINE_D].rotation_degrees = 270;
    
    // Borde - Diagonales (B_DIAG_RU con rotaciones)
    // 0°: RU, 90°: LU, 180°: LD, 270°: RD
    g_textures[TEXTURE_B_DIAG_RU].src_rect = (SDL_Rect){22, 0, TEXTURE_SIZE, TEXTURE_SIZE};
    g_textures[TEXTURE_B_DIAG_RU].base_atlas_index = 2;
    g_textures[TEXTURE_B_DIAG_RU].rotation_degrees = 0;
    
    g_textures[TEXTURE_B_DIAG_LU].src_rect = (SDL_Rect){22, 0, TEXTURE_SIZE, TEXTURE_SIZE};
    g_textures[TEXTURE_B_DIAG_LU].base_atlas_index = 2;
    g_textures[TEXTURE_B_DIAG_LU].rotation_degrees = 90;
    
    g_textures[TEXTURE_B_DIAG_LD].src_rect = (SDL_Rect){22, 0, TEXTURE_SIZE, TEXTURE_SIZE};
    g_textures[TEXTURE_B_DIAG_LD].base_atlas_index = 2;
    g_textures[TEXTURE_B_DIAG_LD].rotation_degrees = 180;
    
    g_textures[TEXTURE_B_DIAG_RD].src_rect = (SDL_Rect){22, 0, TEXTURE_SIZE, TEXTURE_SIZE};
    g_textures[TEXTURE_B_DIAG_RD].base_atlas_index = 2;
    g_textures[TEXTURE_B_DIAG_RD].rotation_degrees = 270;
    
    // Borde - Vía
    g_textures[TEXTURE_B_VIA].src_rect = (SDL_Rect){33, 0, TEXTURE_SIZE, TEXTURE_SIZE};
    g_textures[TEXTURE_B_VIA].base_atlas_index = 3;
    g_textures[TEXTURE_B_VIA].rotation_degrees = 0;
    
    // Relleno - Líneas (F_LINE_R con rotaciones)
    g_textures[TEXTURE_F_LINE_R].src_rect = (SDL_Rect){44, 0, TEXTURE_SIZE, TEXTURE_SIZE};
    g_textures[TEXTURE_F_LINE_R].base_atlas_index = 4;
    g_textures[TEXTURE_F_LINE_R].rotation_degrees = 0;
    
    g_textures[TEXTURE_F_LINE_U].src_rect = (SDL_Rect){44, 0, TEXTURE_SIZE, TEXTURE_SIZE};
    g_textures[TEXTURE_F_LINE_U].base_atlas_index = 4;
    g_textures[TEXTURE_F_LINE_U].rotation_degrees = 90;
    
    g_textures[TEXTURE_F_LINE_L].src_rect = (SDL_Rect){44, 0, TEXTURE_SIZE, TEXTURE_SIZE};
    g_textures[TEXTURE_F_LINE_L].base_atlas_index = 4;
    g_textures[TEXTURE_F_LINE_L].rotation_degrees = 180;
    
    g_textures[TEXTURE_F_LINE_D].src_rect = (SDL_Rect){44, 0, TEXTURE_SIZE, TEXTURE_SIZE};
    g_textures[TEXTURE_F_LINE_D].base_atlas_index = 4;
    g_textures[TEXTURE_F_LINE_D].rotation_degrees = 270;
    
    // Relleno - Diagonales (F_DIAG_RU con rotaciones)
    g_textures[TEXTURE_F_DIAG_RU].src_rect = (SDL_Rect){55, 0, TEXTURE_SIZE, TEXTURE_SIZE};
    g_textures[TEXTURE_F_DIAG_RU].base_atlas_index = 5;
    g_textures[TEXTURE_F_DIAG_RU].rotation_degrees = 0;
    
    g_textures[TEXTURE_F_DIAG_LU].src_rect = (SDL_Rect){55, 0, TEXTURE_SIZE, TEXTURE_SIZE};
    g_textures[TEXTURE_F_DIAG_LU].base_atlas_index = 5;
    g_textures[TEXTURE_F_DIAG_LU].rotation_degrees = 90;
    
    g_textures[TEXTURE_F_DIAG_LD].src_rect = (SDL_Rect){55, 0, TEXTURE_SIZE, TEXTURE_SIZE};
    g_textures[TEXTURE_F_DIAG_LD].base_atlas_index = 5;
    g_textures[TEXTURE_F_DIAG_LD].rotation_degrees = 180;
    
    g_textures[TEXTURE_F_DIAG_RD].src_rect = (SDL_Rect){55, 0, TEXTURE_SIZE, TEXTURE_SIZE};
    g_textures[TEXTURE_F_DIAG_RD].base_atlas_index = 5;
    g_textures[TEXTURE_F_DIAG_RD].rotation_degrees = 270;
    
    // Relleno - Vía
    g_textures[TEXTURE_F_VIA].src_rect = (SDL_Rect){66, 0, TEXTURE_SIZE, TEXTURE_SIZE};
    g_textures[TEXTURE_F_VIA].base_atlas_index = 6;
    g_textures[TEXTURE_F_VIA].rotation_degrees = 0;
    
    // Visual - Corners (V_CORNER_RU con rotaciones)
    g_textures[TEXTURE_V_CORNER_RU].src_rect = (SDL_Rect){77, 0, TEXTURE_SIZE, TEXTURE_SIZE};
    g_textures[TEXTURE_V_CORNER_RU].base_atlas_index = 7;
    g_textures[TEXTURE_V_CORNER_RU].rotation_degrees = 0;
    
    g_textures[TEXTURE_V_CORNER_LU].src_rect = (SDL_Rect){77, 0, TEXTURE_SIZE, TEXTURE_SIZE};
    g_textures[TEXTURE_V_CORNER_LU].base_atlas_index = 7;
    g_textures[TEXTURE_V_CORNER_LU].rotation_degrees = 90;
    
    g_textures[TEXTURE_V_CORNER_LD].src_rect = (SDL_Rect){77, 0, TEXTURE_SIZE, TEXTURE_SIZE};
    g_textures[TEXTURE_V_CORNER_LD].base_atlas_index = 7;
    g_textures[TEXTURE_V_CORNER_LD].rotation_degrees = 180;
    
    g_textures[TEXTURE_V_CORNER_RD].src_rect = (SDL_Rect){77, 0, TEXTURE_SIZE, TEXTURE_SIZE};
    g_textures[TEXTURE_V_CORNER_RD].base_atlas_index = 7;
    g_textures[TEXTURE_V_CORNER_RD].rotation_degrees = 270;
    
    // Visual - Hover
    g_textures[TEXTURE_V_HOVER].src_rect = (SDL_Rect){88, 0, TEXTURE_SIZE, TEXTURE_SIZE};
    g_textures[TEXTURE_V_HOVER].base_atlas_index = 8;
    g_textures[TEXTURE_V_HOVER].rotation_degrees = 0;
}

// -----------------------------------------------
// Cargar assets
// -----------------------------------------------
int assets_load(void) {
    // Cargar atlas
    SDL_Surface *surf = IMG_Load("./resources/textures/atlas.png");
    if (!surf) {
        printf("[Assets] Error al cargar atlas.png\n");
        return 0;
    }

    g_atlas_texture = SDL_CreateTextureFromSurface(ctx.renderer, surf);
    SDL_FreeSurface(surf);

    if (!g_atlas_texture) {
        printf("[Assets] Error al crear textura del atlas\n");
        return 0;
    }

    printf("[Assets] Atlas cargado: %dx%d\n", ATLAS_WIDTH, ATLAS_HEIGHT);

    // Inicializar tabla de texturas
    assets_init_texture_table();
    printf("[Assets] %d texturas precargadas\n", TEXTURE_COUNT);
    return 1;
}

// -----------------------------------------------
// Obtener referencia a textura
// -----------------------------------------------
TextureRef* assets_get_texture(TextureID id) {
    if (id >= 0 && id < TEXTURE_COUNT) {
        return &g_textures[id];
    }
    return NULL;
}

// -----------------------------------------------
// Obtener atlas texture
// -----------------------------------------------
SDL_Texture* assets_get_atlas_texture() {
    return g_atlas_texture;
}

// -----------------------------------------------
// Liberar assets
// -----------------------------------------------
void assets_free() {
    if (g_atlas_texture) {
        SDL_DestroyTexture(g_atlas_texture);
        g_atlas_texture = NULL;
    }
}
