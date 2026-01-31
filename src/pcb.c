// pcb.c - Tablero PCB con grilla de celdas
#include "pcb.h"
#include "celda.h"
#include "context.h"
#include <stdlib.h>

#define CELL_SIZE 33  // Escalado x3 desde 11

// Array para mapear posición de grilla a component ID
static int *g_cell_map = NULL;
static int g_cell_map_size = 0;

// Inicializar PCB
static void pcb_init(Component *comp, int cols, int rows) {
    comp->visible = 1;
    
    // Calcular tamaño total: sin padding, exactamente (cols * CELL_SIZE) x (rows * CELL_SIZE)
    comp->size.width = cols * CELL_SIZE;
    comp->size.height = rows * CELL_SIZE;
}

// Renderizar PCB (solo dibuja las celdas como children)
static int pcb_paint(Component *comp, SDL_Renderer *renderer) {
    // Renderizar children (las celdas)
    component_paint_children(comp, renderer);
    
    return 0;
}

// Crear PCB
PCB* pcb_create(int parentId, Position pos, int cols, int rows) {
    PCB *pcb = (PCB*)malloc(sizeof(PCB));
    if (!pcb) return NULL;
    
    Component *comp = &pcb->base;
    
    // Inicializar como componente (esto registra en el array global)
    component_init(comp);
    
    // Establecer parent
    comp->parentId = parentId;
    
    // Establecer posición
    comp->pos = pos;
    
    // Inicializar datos de PCB
    pcb_init(comp, cols, rows);
    pcb->rows = rows;
    pcb->cols = cols;
    pcb->cell_size = CELL_SIZE;
    
    // Sobrescribir paint con el personalizado del PCB
    comp->paint = pcb_paint;
    
    // Agregar como child del padre si existe
    if (parentId > 0) {
        component_attach_to_parent(comp, parentId);
    }
    
    // Crear mapa de celdas
    g_cell_map_size = rows * cols;
    if (g_cell_map) free(g_cell_map);
    g_cell_map = (int*)malloc(sizeof(int) * g_cell_map_size);
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
