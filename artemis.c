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

void calcularPulso(double aceleracion, double tiempo, ResultadoPulso *resultado) {
    resultado->aceleracion_lateral = aceleracion;
    resultado->tiempo = tiempo;
    resultado->desviacion = 0.5 * aceleracion * tiempo * tiempo;
    resultado->velocidad_lateral = aceleracion * tiempo;
}

void mostrarResultadoPulso(const ResultadoPulso *pulso) {
    printf("\n========== PULSO LATERAL ==========\n");
    printf("Aceleracion lateral: %.2f m/s^2\n", pulso->aceleracion_lateral);
    printf("Duracion del pulso: %.2f segundos\n", pulso->tiempo);
    printf("Desviacion lateral: %.2f metros\n", pulso->desviacion);
    printf("Velocidad lateral alcanzada: %.2f m/s\n", pulso->velocidad_lateral);
    printf("El pulso agrega una componente de velocidad perpendicular a la orbital.\n");
    printf("En el modelo simplificado, la componente lateral tiene aceleracion constante.\n");
    printf("La nave se aparta de la orbita circular original.\n");
    printf("La desviacion corresponde al desplazamiento lateral durante el pulso.\n");
    printf("No se calcula la nueva orbita completa despues de la maniobra.\n");
    printf("===================================\n");
}

void realizarSimulacionPulso(EstadoSimulador *estado) {
    double aceleracion;
    double tiempo;

    printf("\n--- Simulacion del propulsor lateral ---\n");
    aceleracion = leerReal("Aceleracion lateral (m/s^2): ", 0.0, 100.0);
    tiempo = leerReal("Duracion del pulso (segundos): ", 0.0, 300.0);

    calcularPulso(aceleracion, tiempo, &estado->ultimo_pulso);
    mostrarResultadoPulso(&estado->ultimo_pulso);
    estado->hay_pulso = 1;
}
