#include <stdio.h>
#include "validaciones.h"

int leerEnteroSeguro(const char *mensaje, int min, int max) {
    int valor;
    char s;
    while (1) {
        printf("%s", mensaje);
        if (scanf("%d", &valor) == 1) {
            // Limpiar el buffer de entrada por si quedaron caracteres extra
            while ((s = getchar()) != '\n' && s != EOF);
            
            if (valor >= min && valor <= max) {
                return valor;
            } else {
                printf("Error: El valor debe estar entre %d y %d.\n", min, max);
            }
        } else {
            // Limpiar el buffer en caso de que ingresen letras o símbolos
            while ((s = getchar()) != '\n' && s != EOF);
            printf("Error: Ingrese un número entero válido.\n");
        }
    }
}

float leerFlotanteSeguro(const char *mensaje, float min, float max) {
    float valor;
    char s;
    while (1) {
        printf("%s", mensaje);
        if (scanf("%f", &valor) == 1) {
            // Limpiar el buffer de entrada
            while ((s = getchar()) != '\n' && s != EOF);
            
            if (valor >= min && valor <= max) {
                return valor;
            } else {
                printf("Error: El valor debe estar entre %.2f y %.2f.\n", min, max);
            }
        } else {
            // Limpiar el buffer en caso de error
            while ((s = getchar()) != '\n' && s != EOF);
            printf("Error: Ingrese un número válido.\n");
        }
    }
}