#ifndef ARTEMIS_H
#define ARTEMIS_H

#define G 6.674e-11f
#define M_TIERRA 5.972e24f
#define R_TIERRA 6371000.0f

typedef struct {
    float masa;
    float altitud;
    float velocidad;
} Nave;

void mostrarMenu();
void ingresarDatosNave(Nave *nave);
float calcularVelocidadOrbital(float altitud);
float calcularPeriodoOrbital(float altitud);
float calcularEnergiaMecanica(const Nave *nave);

#endif
