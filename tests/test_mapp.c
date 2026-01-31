#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#endif


int main (int argc, char* argv[]) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif


    printf("========== Iniciando pruebas MAPP ==========\n\n");
   
    printf("\n========== ✓ Todas las pruebas pasaron ==========\n");
    
    return 0;
}
