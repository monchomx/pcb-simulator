#include "mapp.h" // SDL2 y MAPP_Context
#include <stdio.h> // printf

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "PCB Zoom Demo"

long iniTimeMS=0;
long endTimeMS=0;

void main_loop(SDL_Window* window,SDL_Renderer* renderer){
    int running=1;
    SDL_Event event;
    MAPP_Context* ctx = MAPP_Init(window,renderer);
    while(running){
        //Si se acumulan eventos, procesarlos
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT){
                running = 0;
                break;
            }else{
                iniTimeMS=SDL_GetTicks();
                MAPP_HandleEvent(ctx,&event);
                endTimeMS=SDL_GetTicks();//para usar en el futuro
            }
        }
        SDL_Delay(16); // Aproximadamente 60 FPS
    }
    MAPP_Destroy(ctx);
}


int SDL_main (int argc, char* argv[]){
    //Inicializar video
    if(SDL_Init(SDL_INIT_VIDEO)){
        printf("Error al incializar video %s \n",SDL_GetError());
        return -1;
    }
    // Crear Ventana y Renderer
    SDL_Window* window = SDL_CreateWindow(WINDOW_TITLE,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    // Iniciar si la ventana y el renderer son válidos
    if( window && renderer ){
        // Iniciar loop principal
        main_loop(window,renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
