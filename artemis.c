#include <stdio.h>
#include <math.h>
#include "artemis.h"
#include "validaciones.h"

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

double calcularVelocidadOrbital(double altitud_m) {
    double r = RADIO_TIERRA + altitud_m;
    return sqrt((G_CONST * MASA_TIERRA) / r);
}

double calcularPeriodoOrbital(double velocidad, double altitud_m) {
    double r = RADIO_TIERRA + altitud_m;
    return (2.0 * M_PI * r) / velocidad;
}

double calcularAceleracionCentripeta(double velocidad, double altitud_m) {
    double r = RADIO_TIERRA + altitud_m;
    return (velocidad * velocidad) / r;
}