// pcb.c - Tablero PCB con grilla de celdas
#include "pcb.h"
#include "celda.h"
#include "context.h"
#include <stdlib.h>

#define CELL_SIZE 11

// Array para mapear posición de grilla a component ID
static int *g_cell_map = NULL;
static int g_cell_map_size = 0;

// Inicializar PCB
static void pcb_init(Component *comp, int rows, int cols) {
    comp->visible = 1;
    
    // Calcular tamaño total: sin padding, exactamente (cols * CELL_SIZE) x (rows * CELL_SIZE)
    comp->size.width = cols * CELL_SIZE;
    comp->size.height = rows * CELL_SIZE;
}

// Renderizar PCB (solo dibuja fondo, las celdas se dibujan como children)
static int pcb_paint(Component *comp, SDL_Renderer *renderer) {
    // Dibujar fondo gris oscuro
    SDL_Rect bg_rect = {comp->pos.x, comp->pos.y, comp->size.width, comp->size.height};
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderFillRect(renderer, &bg_rect);
    
    // Dibujar borde
    SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
    SDL_RenderDrawRect(renderer, &bg_rect);
    
    // Renderizar children (las celdas)
    component_paint_children(comp, renderer);
    
    return 0;
}

// Crear PCB
PCB* pcb_create(int parentId, Position pos, int rows, int cols) {
    PCB *pcb = (PCB*)malloc(sizeof(PCB));
    if (!pcb) return NULL;
    
    Component *comp = &pcb->base;
    comp->parentId = parentId;
    comp->childCount = 0;
    comp->texture = NULL;
    comp->text = NULL;
    comp->font = NULL;
    comp->onClick = NULL;
    comp->onHover = NULL;
    comp->onPress = NULL;
    comp->onRelease = NULL;
    
    comp->update = NULL;
    comp->paint = pcb_paint;
    comp->destroy = (void(*)(Component*))pcb_destroy;
    
    // Establecer posición
    comp->pos = pos;
    
    // Inicializar datos de PCB
    pcb_init(comp, rows, cols);
    pcb->rows = rows;
    pcb->cols = cols;
    pcb->cell_size = CELL_SIZE;
    
    // Registrar en array global
    comp->id = generateId();
    
    // Agregar como child del padre si existe
    if (parentId > 0) {
        component_attach_to_parent(comp, parentId);
    }
    
    // Crear mapa de celdas
    g_cell_map_size = rows * cols;
    if (g_cell_map) free(g_cell_map);
    g_cell_map = (int*)malloc(sizeof(int) * g_cell_map_size);
    
    // Crear todas las celdas
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            int cell_x = pos.x + (c * CELL_SIZE);
            int cell_y = pos.y + (r * CELL_SIZE);
            
            Celda *celda = celda_create(cell_x, cell_y, CELL_SIZE);
            if (celda) {
                // Agregar como child del PCB
                component_attach_to_parent(&celda->base, comp->id);
                
                // Guardar en mapa
                int idx = r * cols + c;
                g_cell_map[idx] = celda->base.id;
            }
        }
    }
    
    return pcb;
}

// Destruir PCB
void pcb_destroy(PCB *pcb) {
    if (!pcb) return;
    
    Component *comp = &pcb->base;
    
    // Remover del array global
    removeComponentFromGlobalArray(comp);
    
    // Liberar children
    for (int i = 0; i < comp->childCount; i++) {
        Component *child = findComponentById(comp->childIds[i]);
        if (child && child->destroy) {
            child->destroy(child);
        }
    }
    
    // Liberar mapa
    if (g_cell_map) {
        free(g_cell_map);
        g_cell_map = NULL;
        g_cell_map_size = 0;
    }
    
    free(pcb);
}

// Obtener component ID de celda por posición
int pcb_get_cell_component_id(PCB *pcb, int row, int col) {
    if (!pcb || row < 0 || col < 0 || row >= pcb->rows || col >= pcb->cols) {
        return 0;
    }
    
    int idx = row * pcb->cols + col;
    if (idx < g_cell_map_size) {
        return g_cell_map[idx];
    }
    
    return 0;
}
