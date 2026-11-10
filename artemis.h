#ifndef ARTEMIS_H
#define ARTEMIS_H

float G = 6.674e-11;
float M_TIERRA = 5.972e24;
float R_TIERRA = 6371000.0;

struct Nave {
    float masa;
    float altitud;
    float velocidad;
};

void mostrarMenu();

#endif
