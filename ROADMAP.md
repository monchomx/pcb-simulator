# PCB Simulator - ROADMAP

## Visión General
Construir un simulador de circuitos impresos (PCB) en C con SDL2, con soporte para:
- Diseño de circuitos en grilla de celdas
- Sistema de texturas para representar trazas, componentes y conexiones
- Simulación eléctrica
- PCB de doble cara

---

## Fases del Proyecto

### ✅ FASE 1: Environment Setup
**Estado:** Completado
- Configurar compilador GCC/MinGW32 UCRT64
- Instalar y configurar SDL2, SDL2_image, SDL2_ttf
- Crear Makefile con reglas de build/clean/run
- **Resultado:** Entorno de compilación funcional

---

### ✅ FASE 2: SDL2 Hello World
**Estado:** Completado
- Crear ventana básica 800×600
- Event loop funcional (SDL_Event)
- Renderizado básico (clear screen, present)
- **Resultado:** Aplicación SDL2 minimalista corriendo

---

### ✅ FASE 3: GUI Component System Architecture
**Estado:** Completado
- Implementar sistema de componentes jerárquico
  - Estructura base `Component` con function pointers
  - Global component registry (array de 1000 componentes)
  - ID-based lookup via `findComponentById()`
- Crear componentes base:
  - `Panel`: contenedor rectangular con color de fondo/borde
  - `Label`: texto renderizado
  - `Button`: botones interactivos
  - `ViewportPanel`: ventana de scroll/zoom
- Parent-child relationships y propagación de eventos
- **Resultado:** Sistema modular de componentes funcionando

---

### 🔄 FASE 4: Atlas Texture System & PCB Grid Rendering
**Estado:** En Progreso (Mostly Complete)

#### 4.1 ✅ Load Atlas & Generate Texture Table
- Cargar `resources/textures/atlas.png` (99×11 píxeles)
- Crear tabla de 24 `TextureID` con referencias pre-calculadas
  - TEXTURE_EMPTY (base color/círculos)
  - B_LINE (bordes) - 4 rotaciones
  - B_DIAG (bordes diagonales) - 4 rotaciones
  - B_VIA (vía de borde)
  - F_LINE (rellenos) - 4 rotaciones
  - F_DIAG (rellenos diagonales) - 4 rotaciones
  - F_VIA (vía de relleno)
  - V_CORNER (esquinas visuales) - 4 rotaciones
  - V_HOVER (estado hover visual)
- **Resultado:** Atlas cargado, tabla de texturas funcional

#### 4.2 ✅ PCB Component Architecture
- Crear componente `PCB` (20×10 celdas con CELL_SIZE=33×33)
- Crear componente `Celda` (celda individual del PCB)
- PCB crea 200 celdas automáticamente en grid
- Todas las celdas inician con TEXTURE_EMPTY
- **Critical Discovery:** Ambos componentes DEBEN llamar `component_init()` para registrarse
- **Resultado:** 200 celdas creadas, registradas, y renderizadas

#### 4.3 ✅ Texture Rendering from Atlas
- Implementar `celda_paint()` para renderizar desde atlas
- Usar `SDL_RenderCopy()` para copiar src_rect del atlas al dest_rect en pantalla
- Soportar `SDL_RenderCopyEx()` para rotaciones (rotation_degrees)
- Escalamiento automático: 11×11 atlas → 33×33 pantalla (x3)
- **Resultado:** Grid de 200 celdas dibujando texturas correctamente

#### 4.4 🔲 Interactive Texture Testing (TODO)
- Mouse click en celda para cambiar textura
- Ciclar a través de todas las 24 TextureID
- Visualizar todas las texturas disponibles
- Verificar rotaciones funcionan correctamente
- **Objetivo:** Validar que todas las 24 texturas son correctas

---

### ❌ FASE 5: Trace Drawing System
**Estado:** No iniciado
- Sistema de trazado de circuitos (Path following)
- Usuario dibuja líneas en el PCB conectando puntos
- Soporte para:
  - Líneas horizontales/verticales
  - Líneas diagonales
  - Vías (conexiones perpendiculares)
  - Cambio dinámico de textura según trazado
- **Objetivo:** Poder dibujar circuitos en el PCB

---

### ❌ FASE 6: Double-Sided PCB
**Estado:** No iniciado
- Expandir a PCB de doble cara (front/back)
- Layer toggle para ver cara frontal vs posterior
- Vías que conectan ambas caras
- **Objetivo:** Soporte para circuitos más complejos

---

### ❌ FASE 7: Electrical Simulation
**Estado:** No iniciado
- Simulación de circuitos eléctricos
- Detección de nodos y net connectivity
- Validación de circuitos
- Análisis de voltaje/corriente (opcional)
- **Objetivo:** Validación y análisis de circuitos

---

## Estado Actual (30-Enero-2026)

### ✅ Completado
- [x] Environment setup
- [x] SDL2 basic rendering
- [x] Component system (hierarchical, ID-based, event propagation)
- [x] GUI components (Panel, Label, Button, Viewport)
- [x] Atlas loading (99×11, 24 texturas indexadas)
- [x] PCB grid creation (10 cols × 20 rows)
- [x] Celda rendering with atlas textures
- [x] Texture escalamiento x3 (11→33 píxeles)

### 🔄 En Progreso
- [ ] Interactive texture testing (Phase 4.4)

### 📋 Próximos Pasos (Prioridad)
1. **Phase 4.4:** Agregar click de mouse para cambiar texturas y validar todas las 24
2. **Phase 5:** Implementar trace drawing system
3. **Phase 6:** PCB de doble cara
4. **Phase 7:** Simulación eléctrica

---

## Arquitectura Técnica

### Stack
- **Lenguaje:** C
- **Gráficos:** SDL2, SDL2_image, SDL2_ttf
- **Compilador:** GCC/MinGW32 UCRT64
- **Build System:** Makefile

### Estructura de Directorios
```
MyApp/
├── bin/              # Ejecutables
├── build/            # Objetos compilados
├── include/          # Headers
├── src/              # Código fuente
├── resources/
│   ├── fonts/
│   └── textures/
├── tests/            # Unit tests
├── Makefile
└── ROADMAP.md        # Este archivo
```

### Pattern Crítico: Component Initialization
```c
// ✅ CORRECTO - Siempre seguir este patrón
ComponentType* component_create(...) {
    ComponentType *comp = malloc(sizeof(ComponentType));
    Component *base = &comp->base;
    
    // 1. SIEMPRE llamar component_init() PRIMERO
    component_init(base);
    
    // 2. Sobrescribir function pointers necesarios
    base->paint = custom_paint;
    base->destroy = custom_destroy;
    
    // 3. Establecer datos específicos
    comp->custom_field = value;
    
    return comp;
}

// ❌ INCORRECTO - Inicialización manual sin component_init()
// Causará que function pointers no se ejecuten
```

---

## Performance Notes
- Max components: 1000 (MAX_COMPONENTS en component.c)
- Current components: ~205 (1 root + 1 viewport + 1 canvas + 1 PCB + 200 celdas + 1 extra)
- CELL_SIZE: 33×33 píxeles (escalado x3 desde atlas de 11×11)
- Grid: 10 columnas × 20 renglones = 330×660 píxeles

---

## Referencias de Texturas (Atlas Layout)
```
Atlas: 99×11 píxeles
9 texturas base de 11×11 cada una:
[0] EMPTY     [1] B_LINE    [2] B_DIAG    
[3] B_VIA     [4] F_LINE    [5] F_DIAG    
[6] F_VIA     [7] V_CORNER  [8] V_HOVER

24 TextureID totales (con rotaciones):
TEXTURE_EMPTY         (1 base)
TEXTURE_B_LINE_0°, 90°, 180°, 270°
TEXTURE_B_DIAG_0°, 90°, 180°, 270°
TEXTURE_B_VIA         (1 base)
TEXTURE_F_LINE_0°, 90°, 180°, 270°
TEXTURE_F_DIAG_0°, 90°, 180°, 270°
TEXTURE_F_VIA         (1 base)
TEXTURE_V_CORNER_0°, 90°, 180°, 270°
TEXTURE_V_HOVER       (1 base)
```

---

## Notas Importantes

### Debugging
- Debug prints removidos de rendering loop (demasiado verbose)
- Usar `make run` para compilar y ejecutar en un paso
- Exit code 124 en timeout = app ejecutándose correctamente (interrupted)

### Cambios Recientes
- CELL_SIZE: 11 → 33 (escalamiento x3)
- Parámetros pcb_create: (rows, cols) → **(cols, rows)** - columnas primero
- celda_create ahora usa component_init() correctamente

---

**Last Updated:** 30-Enero-2026 00:00 UTC
