#include <stdio.h>
#include <math.h>
#include "artemis.h"

struct Nave inicializarNave() {
    struct Nave n;
    n.masa = 33000.0f;
    n.altitud = 192000.0f;
    n.velocidad = 0.0f;
    return n;
}

void mostrarMenu() {
    printf("\n=== MENU DE NAVEGACION ARTEMIS ===\n");
    printf("1. Ejecutar Simubloque LEO\n");
    printf("2. Salir\n");
}