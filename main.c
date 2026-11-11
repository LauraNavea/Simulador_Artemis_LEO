#include <stdio.h>
#include "artemis.h"
int main() {
    int opcion;
    struct Nave orion;
    orion.masa = 33000.0;
    orion.altitud = 192000.0;
    printf("--- SIMULADOR ARTEMIS LEO (VERSION INICIAL) ---\n");
    printf("1. Calcular Velocidad Orbital\n");
    printf("2. Salir\n");
    printf("Ingrese opcion: ");
    
    scanf("%d", &opcion);
    if (opcion == 1) {
        float r = R_TIERRA + orion.altitud;
        float v = 7784.0;
        printf("Velocidad calculada: %.2f m/s\n", v);
    }
    return 0;
}