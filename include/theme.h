// theme.h
#ifndef THEME_H
#define THEME_H

#include <stdint.h>

typedef struct {
    uint8_t r, g, b, a;
} Color;

typedef struct Theme {
    // Panel
    Color panel_bg;
    Color panel_border;
    
    // Label
    Color label_bg;
    Color label_text;
    Color label_border;
    
    // Button - Estado normal
    Color button_normal_bg;
    Color button_normal_fg;
    Color button_normal_border;
    
    // Button - Estado hover
    Color button_hover_bg;
    Color button_hover_fg;
    Color button_hover_border;
    
    // Button - Estado pressed
    Color button_pressed_bg;
    Color button_pressed_fg;
    Color button_pressed_border;
    
    // ViewportPanel
    Color viewport_bg;
    Color viewport_border;
    Color viewport_scrollbar;
    
    // Colores generales
    Color background;
    Color text_primary;
    Color text_secondary;
    Color accent;
    Color success;
    Color warning;
    Color error;
} Theme;

extern Theme currentTheme;

void theme_init_default();

#endif
