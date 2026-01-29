#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "mapp.h"
#ifdef _WIN32
#include <windows.h>
#endif

// ================== Test MAPP_Init ==================
void test_MAPP_Init_allocates_memory(void) {
    MAPP_Context* ctx = MAPP_Init(NULL, NULL);
    
    assert(ctx != NULL && "MAPP_Init debe retornar un contexto válido");
    
    MAPP_Destroy(ctx);
    printf("✓ test_MAPP_Init_allocates_memory\n");
}

void test_MAPP_Init_initializes_fields(void) {
    MAPP_Context* ctx = MAPP_Init(NULL, NULL);
    
    assert(ctx->window == NULL && "window debe ser NULL");
    assert(ctx->renderer == NULL && "renderer debe ser NULL");
    assert(ctx->inputFlags == 0 && "inputFlags debe ser 0");
    assert(ctx->zoomLevel == 100 && "zoomLevel debe ser 100");
    assert(ctx->wheelValue == 0 && "wheelValue debe ser 0");
    
    // Verificar coordenadas del mouse
    assert(ctx->mouseCoords.startX == 0 && "mouseCoords.startX debe ser 0");
    assert(ctx->mouseCoords.startY == 0 && "mouseCoords.startY debe ser 0");
    assert(ctx->mouseCoords.endX == 0 && "mouseCoords.endX debe ser 0");
    assert(ctx->mouseCoords.endY == 0 && "mouseCoords.endY debe ser 0");
    assert(ctx->mouseCoords.currentX == 0 && "mouseCoords.currentX debe ser 0");
    assert(ctx->mouseCoords.currentY == 0 && "mouseCoords.currentY debe ser 0");
    
    MAPP_Destroy(ctx);
    printf("✓ test_MAPP_Init_initializes_fields\n");
}

// ================== Test MAPP_Destroy ==================
void test_MAPP_Destroy_handles_null(void) {
    // No debe causar segfault
    MAPP_Destroy(NULL);
    printf("✓ test_MAPP_Destroy_handles_null\n");
}

void test_MAPP_Destroy_frees_memory(void) {
    MAPP_Context* ctx = MAPP_Init(NULL, NULL);
    assert(ctx != NULL);
    
    MAPP_Destroy(ctx);
    // Después de destroy, ctx apunta a memoria liberada
    // (no podemos probar directamente, pero al menos no hay segfault)
    printf("✓ test_MAPP_Destroy_frees_memory\n");
}

// ================== Test MAPP_ProcessMSEvent ==================
void test_MAPP_ProcessMSEvent_left_button_down(void) {
    MAPP_Context* ctx = MAPP_Init(NULL, NULL);
    
    SDL_Event event = {0};
    event.type = SDL_MOUSEBUTTONDOWN;
    event.button.button = SDL_BUTTON_LEFT;
    event.button.x = 100;
    event.button.y = 200;
    
    MAPP_ProcessMSEvent(ctx, &event);
    
    assert(ctx->inputFlags & FLAG_LEFT_MSBTN_DOWN && "FLAG_LEFT_MSBTN_DOWN debe estar activado");
    assert(!(ctx->inputFlags & FLAG_LEFT_MSBTN_UP) && "FLAG_LEFT_MSBTN_UP debe estar desactivado");
    assert(ctx->mouseCoords.startX == 100 && "startX debe ser 100");
    assert(ctx->mouseCoords.startY == 200 && "startY debe ser 200");
    
    MAPP_Destroy(ctx);
    printf("✓ test_MAPP_ProcessMSEvent_left_button_down\n");
}

void test_MAPP_ProcessMSEvent_right_button_down(void) {
    MAPP_Context* ctx = MAPP_Init(NULL, NULL);
    
    SDL_Event event = {0};
    event.type = SDL_MOUSEBUTTONDOWN;
    event.button.button = SDL_BUTTON_RIGHT;
    event.button.x = 50;
    event.button.y = 75;
    
    MAPP_ProcessMSEvent(ctx, &event);
    
    assert(ctx->inputFlags & FLAG_RIGHT_MSBTN_DOWN && "FLAG_RIGHT_MSBTN_DOWN debe estar activado");
    assert(!(ctx->inputFlags & FLAG_RIGHT_MSBTN_UP) && "FLAG_RIGHT_MSBTN_UP debe estar desactivado");
    assert(ctx->mouseCoords.startX == 50 && "startX debe ser 50");
    assert(ctx->mouseCoords.startY == 75 && "startY debe ser 75");
    
    MAPP_Destroy(ctx);
    printf("✓ test_MAPP_ProcessMSEvent_right_button_down\n");
}

void test_MAPP_ProcessMSEvent_left_button_up(void) {
    MAPP_Context* ctx = MAPP_Init(NULL, NULL);
    ctx->inputFlags |= FLAG_LEFT_MSBTN_DOWN; // Simulamos que el botón estaba presionado
    
    SDL_Event event = {0};
    event.type = SDL_MOUSEBUTTONUP;
    event.button.button = SDL_BUTTON_LEFT;
    event.button.x = 150;
    event.button.y = 250;
    
    MAPP_ProcessMSEvent(ctx, &event);
    
    assert(ctx->inputFlags & FLAG_LEFT_MSBTN_UP && "FLAG_LEFT_MSBTN_UP debe estar activado");
    assert(!(ctx->inputFlags & FLAG_LEFT_MSBTN_DOWN) && "FLAG_LEFT_MSBTN_DOWN debe estar desactivado");
    assert(ctx->mouseCoords.endX == 150 && "endX debe ser 150");
    assert(ctx->mouseCoords.endY == 250 && "endY debe ser 250");
    
    MAPP_Destroy(ctx);
    printf("✓ test_MAPP_ProcessMSEvent_left_button_up\n");
}

void test_MAPP_ProcessMSEvent_mouse_motion(void) {
    MAPP_Context* ctx = MAPP_Init(NULL, NULL);
    
    SDL_Event event = {0};
    event.type = SDL_MOUSEMOTION;
    event.motion.x = 320;
    event.motion.y = 240;
    
    MAPP_ProcessMSEvent(ctx, &event);
    
    assert(ctx->inputFlags & FLAG_MOUSE_MOTION && "FLAG_MOUSE_MOTION debe estar activado");
    assert(ctx->mouseCoords.currentX == 320 && "currentX debe ser 320");
    assert(ctx->mouseCoords.currentY == 240 && "currentY debe ser 240");
    
    MAPP_Destroy(ctx);
    printf("✓ test_MAPP_ProcessMSEvent_mouse_motion\n");
}

void test_MAPP_ProcessMSEvent_mouse_wheel(void) {
    MAPP_Context* ctx = MAPP_Init(NULL, NULL);
    
    SDL_Event event = {0};
    event.type = SDL_MOUSEWHEEL;
    event.wheel.y = 3; // Scroll arriba
    
    MAPP_ProcessMSEvent(ctx, &event);
    
    assert(ctx->inputFlags & FLAG_MOUSE_WHEEL && "FLAG_MOUSE_WHEEL debe estar activado");
    assert(ctx->wheelValue == 3 && "wheelValue debe ser 3");
    
    MAPP_Destroy(ctx);
    printf("✓ test_MAPP_ProcessMSEvent_mouse_wheel\n");
}

// ================== Test MAPP_ProcessKBEvent ==================
void test_MAPP_ProcessKBEvent_ctrl_key_down(void) {
    MAPP_Context* ctx = MAPP_Init(NULL, NULL);
    
    SDL_Event event = {0};
    event.type = SDL_KEYDOWN;
    event.key.keysym.sym = SDLK_LCTRL;
    
    MAPP_ProcessKBEvent(ctx, &event);
    
    assert(ctx->inputFlags & FLAG_CTRL_DOWN && "FLAG_CTRL_DOWN debe estar activado");
    assert(!(ctx->inputFlags & FLAG_CTRL_UP) && "FLAG_CTRL_UP debe estar desactivado");
    
    MAPP_Destroy(ctx);
    printf("✓ test_MAPP_ProcessKBEvent_ctrl_key_down\n");
}

void test_MAPP_ProcessKBEvent_ctrl_key_up(void) {
    MAPP_Context* ctx = MAPP_Init(NULL, NULL);
    ctx->inputFlags |= FLAG_CTRL_DOWN; // Simulamos que Ctrl estaba presionado
    
    SDL_Event event = {0};
    event.type = SDL_KEYUP;
    event.key.keysym.sym = SDLK_RCTRL;
    
    MAPP_ProcessKBEvent(ctx, &event);
    
    assert(ctx->inputFlags & FLAG_CTRL_UP && "FLAG_CTRL_UP debe estar activado");
    assert(!(ctx->inputFlags & FLAG_CTRL_DOWN) && "FLAG_CTRL_DOWN debe estar desactivado");
    
    MAPP_Destroy(ctx);
    printf("✓ test_MAPP_ProcessKBEvent_ctrl_key_up\n");
}

int SDL_main (int argc, char* argv[]) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif


    printf("========== Iniciando pruebas MAPP ==========\n\n");
    
    printf("--- MAPP_Init ---\n");
    test_MAPP_Init_allocates_memory();
    test_MAPP_Init_initializes_fields();
    
    printf("\n--- MAPP_Destroy ---\n");
    test_MAPP_Destroy_handles_null();
    test_MAPP_Destroy_frees_memory();
    
    printf("\n--- MAPP_ProcessMSEvent ---\n");
    test_MAPP_ProcessMSEvent_left_button_down();
    test_MAPP_ProcessMSEvent_right_button_down();
    test_MAPP_ProcessMSEvent_left_button_up();
    test_MAPP_ProcessMSEvent_mouse_motion();
    test_MAPP_ProcessMSEvent_mouse_wheel();
    
    printf("\n--- MAPP_ProcessKBEvent ---\n");
    test_MAPP_ProcessKBEvent_ctrl_key_down();
    test_MAPP_ProcessKBEvent_ctrl_key_up();
    
    printf("\n========== ✓ Todas las pruebas pasaron ==========\n");
    
    return 0;
}
