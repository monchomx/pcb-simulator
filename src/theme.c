// theme.c
#include "theme.h"

Theme currentTheme;

void theme_init_default() {
    // Panel
    currentTheme.panel_bg      = (Color){45, 45, 48, 255};     // Gris oscuro
    currentTheme.panel_border  = (Color){63, 63, 70, 255};     // Gris medio oscuro
    
    // Label
    currentTheme.label_bg      = (Color){37, 37, 38, 255};     // Gris más oscuro
    currentTheme.label_text    = (Color){220, 220, 220, 255};  // Blanco cálido
    currentTheme.label_border  = (Color){60, 60, 60, 255};     // Gris sutil
    
    // Button - Estado normal
    currentTheme.button_normal_bg     = (Color){0, 122, 204, 255};   // Azul primary
    currentTheme.button_normal_fg     = (Color){255, 255, 255, 255}; // Blanco
    currentTheme.button_normal_border = (Color){0, 100, 180, 255};   // Azul oscuro
    
    // Button - Estado hover
    currentTheme.button_hover_bg     = (Color){28, 151, 234, 255};  // Azul claro
    currentTheme.button_hover_fg     = (Color){255, 255, 255, 255}; // Blanco
    currentTheme.button_hover_border = (Color){0, 122, 204, 255};   // Azul primary
    
    // Button - Estado pressed
    currentTheme.button_pressed_bg     = (Color){0, 90, 158, 255};   // Azul oscuro
    currentTheme.button_pressed_fg     = (Color){200, 200, 200, 255}; // Gris claro
    currentTheme.button_pressed_border = (Color){0, 70, 130, 255};   // Azul muy oscuro
    
    // ViewportPanel
    currentTheme.viewport_bg        = (Color){50, 50, 50, 255};    // Negro oscuro
    currentTheme.viewport_border    = (Color){80, 80, 80, 255};    // Gris claro
    currentTheme.viewport_scrollbar = (Color){100, 100, 100, 255}; // Gris medio
    
    // Colores generales
    currentTheme.background      = (Color){30, 30, 30, 255};     // Fondo general
    currentTheme.text_primary    = (Color){230, 230, 230, 255};  // Texto principal
    currentTheme.text_secondary  = (Color){150, 150, 150, 255};  // Texto secundario
    currentTheme.accent          = (Color){0, 122, 204, 255};    // Color acento
    currentTheme.success         = (Color){16, 185, 129, 255};   // Verde éxito
    currentTheme.warning         = (Color){245, 158, 11, 255};   // Naranja advertencia
    currentTheme.error           = (Color){239, 68, 68, 255};    // Rojo error
}
