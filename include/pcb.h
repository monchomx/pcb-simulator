#ifndef PCB_H
#define PCB_H

#include "component.h"

// PCB: Tablero de circuitos impreso
// Contiene una grilla de Celda componentes

typedef struct {
    Component base;
    int rows;
    int cols;
    int cell_size;
} PCB;

// Crear PCB (siguiendo patrón de componentes: parentId, pos, rows, cols)
PCB* pcb_create(int parentId, Position pos, int rows, int cols);

// Destruir PCB
void pcb_destroy(PCB *pcb);

// Obtener referencia a celda por posición
int pcb_get_cell_component_id(PCB *pcb, int row, int col);

#endif // PCB_H
