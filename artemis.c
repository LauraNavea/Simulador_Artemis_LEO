#include <stdio.h>
#include <math.h>
#include "artemis.h"
#include "validaciones.h"

#define PI 3.14159265358979323846

void mostrarMenu(void) {
    printf("\n========================================\n");
    printf("       SIMULADOR DE MISION ARTEMIS\n");
    printf("       Insercion orbital - LEO\n");
    printf("========================================\n");
    printf("1. Configurar datos de la mision\n");
    printf("2. Calcular y mostrar orbita\n");
    printf("3. Simular pulso del propulsor lateral\n");
    printf("4. Ver resumen de la mision\n");
    printf("5. Iniciar desafio de la mision\n");
    printf("0. Salir\n");
}

void inicializarSimulador(EstadoSimulador *estado) {
    int i;

    estado->mision.altitud = 192000.0;
    estado->mision.masa_nave = 33000.0;
    estado->cantidad_resultados = 0;
    estado->hay_calculo_vigente = 0;
    estado->hay_pulso = 0;

    for (i = 0; i < MAX_HISTORIAL; i++) {
        estado->historial[i].mision.altitud = 0.0;
        estado->historial[i].mision.masa_nave = 0.0;
        estado->historial[i].resultado.velocidad = 0.0;
        estado->historial[i].resultado.periodo_segundos = 0.0;
        estado->historial[i].resultado.periodo_minutos = 0.0;
        estado->historial[i].resultado.aceleracion_centripeta = 0.0;
        estado->historial[i].resultado.fuerza_gravitatoria = 0.0;
        estado->historial[i].resultado.fuerza_centripeta = 0.0;
    }
}

void configurarMision(EstadoSimulador *estado) {
    printf("\n--- Configuracion de la mision ---\n");
    estado->mision.altitud = leerReal("Altitud sobre la Tierra (m): ",
                                      0.01, 2000000.0);
    estado->mision.masa_nave = leerReal("Masa de Orion (kg): ",
                                        1.0, 1000000.0);

    estado->hay_calculo_vigente = 0;
    printf("Datos actualizados correctamente.\n");
    printf("Los calculos anteriores se conservan solamente en el historial.\n");
}

int validarDatosMision(const DatosMision *mision) {
    if (mision == NULL) {
        return 0;
    }

    if (!(mision->altitud > 0.0) || mision->altitud > 2000000.0) {
        return 0;
    }

    if (!(mision->masa_nave > 0.0) || mision->masa_nave > 1000000.0) {
        return 0;
    }

    return 1;
}

void calcularOrbita(const DatosMision *mision, ResultadoOrbital *resultado) {
    double radio;
    double producto;

    radio = RADIO_TIERRA + mision->altitud;
    producto = G * MASA_TIERRA;

    resultado->velocidad = sqrt(producto / radio);
    resultado->periodo_segundos = (2.0 * PI * radio) / resultado->velocidad;
    resultado->periodo_minutos = resultado->periodo_segundos / 60.0;
    resultado->aceleracion_centripeta =
        resultado->velocidad * resultado->velocidad / radio;

    resultado->fuerza_gravitatoria =
        (G * MASA_TIERRA * mision->masa_nave) / (radio * radio);
    resultado->fuerza_centripeta =
        mision->masa_nave * resultado->velocidad * resultado->velocidad / radio;
}

void calcularPulso(double aceleracion, double tiempo, ResultadoPulso *resultado) {
    resultado->aceleracion_lateral = aceleracion;
    resultado->tiempo = tiempo;
    resultado->desviacion = 0.5 * aceleracion * tiempo * tiempo;
    resultado->velocidad_lateral = aceleracion * tiempo;
}

void mostrarResultadosOrbitales(const DatosMision *mision,
                                const ResultadoOrbital *resultado) {
    printf("\n========== ESTADO DE LA ORBITA ==========\n");
    printf("Altitud: %.2f km\n", mision->altitud / 1000.0);
    printf("Masa de Orion: %.2f kg\n", mision->masa_nave);
    printf("Radio orbital: %.2f km\n",
           (RADIO_TIERRA + mision->altitud) / 1000.0);
    printf("Velocidad orbital: %.2f m/s\n", resultado->velocidad);
    printf("Periodo orbital: %.2f segundos\n", resultado->periodo_segundos);
    printf("Periodo orbital: %.2f minutos\n", resultado->periodo_minutos);
    printf("Aceleracion centripeta: %.4f m/s^2\n",
           resultado->aceleracion_centripeta);
    printf("Fuerza gravitatoria: %.2f N\n", resultado->fuerza_gravitatoria);
    printf("Fuerza centripeta: %.2f N\n", resultado->fuerza_centripeta);
    printf("La fuerza gravitatoria proporciona la fuerza centripeta.\n");
    printf("Igualando sus modulos: Fg = Fc.\n");
    printf("G * MT * m / r^2 = m * v^2 / r.\n");
    printf("Al simplificar la masa de Orion: v = sqrt(G * MT / r).\n");
    printf("Las fuerzas calculadas coinciden aproximadamente por redondeo.\n");
    printf("=========================================\n");
}

void realizarCalculoOrbital(EstadoSimulador *estado) {
    int i;

    if (!validarDatosMision(&estado->mision)) {
        printf("Los datos de la mision no son validos.\n");
        return;
    }

    calcularOrbita(&estado->mision, &estado->ultimo_calculo);
    mostrarResultadosOrbitales(&estado->mision, &estado->ultimo_calculo);

    if (estado->cantidad_resultados < MAX_HISTORIAL) {
        i = estado->cantidad_resultados;
        estado->cantidad_resultados++;
    } else {
        for (i = 0; i < MAX_HISTORIAL - 1; i++) {
            estado->historial[i] = estado->historial[i + 1];
        }
        i = MAX_HISTORIAL - 1;
    }

    estado->historial[i].mision = estado->mision;
    estado->historial[i].resultado = estado->ultimo_calculo;

    estado->hay_calculo_vigente = 1;
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

void mostrarResultadoPulso(const ResultadoPulso *pulso) {
    printf("\n========== PULSO LATERAL ==========\n");
    printf("Aceleracion lateral: %.2f m/s^2\n",
           pulso->aceleracion_lateral);
    printf("Duracion del pulso: %.2f segundos\n", pulso->tiempo);
    printf("Desviacion lateral: %.2f metros\n", pulso->desviacion);
    printf("Velocidad lateral alcanzada: %.2f m/s\n",
           pulso->velocidad_lateral);
    printf("El pulso agrega una componente de velocidad perpendicular a la orbital.\n");
    printf("En el modelo simplificado, la componente lateral tiene aceleracion constante.\n");
    printf("La nave se aparta de la orbita circular original.\n");
    printf("La desviacion corresponde al desplazamiento lateral durante el pulso.\n");
    printf("No se calcula la nueva orbita completa despues de la maniobra.\n");
    printf("===================================\n");
}


void iniciarJuego(EstadoSimulador *estado) {
    int puntaje = 0;
    int integridad = 100;
    int errores = 0;
    int respuesta;
    double aceleracion;
    double tiempo;
    ResultadoPulso pulso;
    char *decision_formula;
    char *decision_fuerzas;
    char *decision_pulso;
    char *decision_recuperacion;

    printf("\n========== DESAFIO DE MISION ARTEMIS ==========\n");
    printf("La tripulacion debe tomar decisiones para mantener segura a Orion.\n");
    printf("Puntaje maximo: 100 puntos. Integridad inicial: 100.\n");
    printf("Las decisiones incorrectas reducen la integridad de la mision.\n");

    printf("\nTURNO 1 - INSERCION ORBITAL\n");
    printf("El equipo debe seleccionar la expresion para calcular la velocidad:\n");
    printf("1. v = sqrt(G * MT / r)\n");
    printf("2. v = sqrt(G * MT / (r * r))\n");
    printf("3. v = G * MT / r\n");
    respuesta = leerEntero("Ingrese su respuesta: ", 1, 3);

    if (respuesta == 1) {
        puntaje = puntaje + 25;
        decision_formula = "Selecciono la formula correcta.";
        printf("Correcto. +25 puntos.\n");
    } else {
        errores++;
        integridad = integridad - 15;
        decision_formula = "Selecciono una formula incorrecta.";
        printf("Incorrecto. -15 de integridad. La formula correcta es v = sqrt(G * MT / r).\n");
    }

    printf("\nSe calculan los parametros orbitales de Orion:\n");
    realizarCalculoOrbital(estado);

    printf("\nTURNO 2 - EVALUACION DE LA ORBITA\n");
    printf("Velocidad calculada: %.2f m/s\n", estado->ultimo_calculo.velocidad);
    printf("Aceleracion centripeta: %.4f m/s2\n",
           estado->ultimo_calculo.aceleracion_centripeta);
    printf("Que debe hacer la tripulacion para mantener la orbita circular ideal?\n");
    printf("1. Mantener motores encendidos continuamente para generar la fuerza centripeta.\n");
    printf("2. Reconocer que la gravedad proporciona la fuerza centripeta.\n");
    respuesta = leerEntero("Ingrese su decision: ", 1, 2);

    if (respuesta == 2) {
        puntaje = puntaje + 25;
        decision_fuerzas = "Reconocio que la gravedad actua como fuerza centripeta.";
        printf("Decision correcta. Fg = Fc. +25 puntos.\n");
    } else {
        errores++;
        integridad = integridad - 20;
        decision_fuerzas = "Propuso mantener los motores encendidos sin necesidad.";
        printf("Decision incorrecta. -20 de integridad. En la orbita ideal la gravedad aporta Fc.\n");
    }

    printf("\nTURNO 3 - MANIOBRA DEL PROPULSOR LATERAL\n");
    printf("El control recomienda a = 0.50 m/s2 durante 10 segundos.\n");
    printf("Ese pulso produce una desviacion lateral de referencia de 25 m.\n");
    printf("1. Ejecutar el pulso recomendado\n");
    printf("2. Ingresar otros valores de aceleracion y duracion\n");
    respuesta = leerEntero("Seleccione una opcion: ", 1, 2);

    if (respuesta == 1) {
        aceleracion = 0.50;
        tiempo = 10.0;
        decision_pulso = "Ejecuto el pulso recomendado (0.50 m/s2 durante 10 s).";
    } else {
        aceleracion = leerReal("Aceleracion lateral (m/s2): ", 0.0, 100.0);
        tiempo = leerReal("Duracion del pulso (segundos): ", 0.0, 300.0);
        decision_pulso = "Configuro un pulso lateral personalizado.";
    }

    calcularPulso(aceleracion, tiempo, &pulso);
    mostrarResultadoPulso(&pulso);
    estado->ultimo_pulso = pulso;
    estado->hay_pulso = 1;

    if (pulso.desviacion >= 0.0 && pulso.desviacion <= 25.0) {
        puntaje = puntaje + 30;
        printf("Desviacion dentro del limite operativo del juego. +30 puntos.\n");
    } else {
        errores++;
        integridad = integridad - 30;
        printf("Desviacion fuera del limite operativo del juego. -30 de integridad.\n");
        printf("La desviacion supera el umbral definido para este desafio.\n");
    }

    printf("\nTURNO 4 - RESPUESTA ANTE LA MANIOBRA\n");
    printf("Desviacion lateral registrada: %.2f m\n", pulso.desviacion);
    printf("Umbral operativo definido para el juego: 25 m.\n");
    printf("Analice el resultado y seleccione la respuesta de la tripulacion.\n");
    printf("1. Mantener el plan de mision\n");
    printf("2. Cancelar nuevas maniobras laterales y activar contingencia\n");
    respuesta = leerEntero("Ingrese su decision: ", 1, 2);

    if ((pulso.desviacion > 25.0 && respuesta == 2) ||
        (pulso.desviacion <= 25.0 && respuesta == 1)) {
        puntaje = puntaje + 20;
        decision_recuperacion = (respuesta == 2)
            ? "Cancelo nuevas maniobras laterales ante una desviacion excesiva."
            : "Decidio mantener el plan con una desviacion dentro del limite.";
        printf("Decision operativa adecuada. +20 puntos.\n");
    } else {
        errores++;
        integridad = integridad - 20;
        decision_recuperacion = (respuesta == 1)
            ? "Mantuvo el plan pese a superar el limite de desviacion."
            : "Cancelo nuevas maniobras laterales aunque la desviacion estaba dentro del limite.";
        printf("Decision operativa inadecuada. -20 de integridad.\n");
    }

    if (integridad < 0) {
        integridad = 0;
    }

    printf("\n========== REPORTE FINAL DE LA MISION ==========\n");
    printf("Puntaje obtenido: %d de 100\n", puntaje);
    printf("Integridad operativa final: %d de 100\n", integridad);
    printf("Cantidad de errores: %d\n", errores);
    printf("\nDECISIONES REGISTRADAS:\n");
    printf("1. Insercion orbital: %s\n", decision_formula);
    printf("2. Evaluacion de fuerzas: %s\n", decision_fuerzas);
    printf("3. Propulsor lateral: %s\n", decision_pulso);
    printf("4. Respuesta operativa: %s\n", decision_recuperacion);
    printf("\nRESULTADOS FISICOS:\n");
    printf("Altitud: %.2f km | Masa de Orion: %.2f kg\n",
           estado->mision.altitud / 1000.0, estado->mision.masa_nave);
    printf("Velocidad orbital: %.2f m/s\n", estado->ultimo_calculo.velocidad);
    printf("Periodo orbital: %.2f s (%.2f min)\n",
           estado->ultimo_calculo.periodo_segundos,
           estado->ultimo_calculo.periodo_minutos);
    printf("Aceleracion centripeta: %.4f m/s2\n",
           estado->ultimo_calculo.aceleracion_centripeta);
    printf("Fuerza gravitatoria: %.2f N | Fuerza centripeta: %.2f N\n",
           estado->ultimo_calculo.fuerza_gravitatoria,
           estado->ultimo_calculo.fuerza_centripeta);
    printf("Desviacion lateral: %.2f m | Velocidad lateral: %.2f m/s\n",
           pulso.desviacion, pulso.velocidad_lateral);

    if (puntaje >= 70 && integridad > 0) {
        printf("\nRESULTADO: DESAFIO SUPERADO. La tripulacion tomo decisiones adecuadas segun las reglas del juego.\n");
    } else {
        printf("\nRESULTADO: MISION NO SUPERADA. La tripulacion debe revisar sus decisiones.\n");
    }
    printf("El desafio puede repetirse desde el menu principal.\n");
    printf("================================================\n");
}

void mostrarResumenMision(const EstadoSimulador *estado) {
    printf("\n========== RESUMEN DE LA MISION ==========\n");
    printf("Altitud configurada: %.2f km\n", estado->mision.altitud / 1000.0);
    printf("Masa de Orion: %.2f kg\n", estado->mision.masa_nave);
    printf("Calculos orbitales guardados en historial: %d\n",
           estado->cantidad_resultados);

    if (estado->cantidad_resultados > 0) {
        int i;
        printf("\nHistorial de calculos (del mas antiguo al mas reciente):\n");
        for (i = 0; i < estado->cantidad_resultados; i++) {
            printf("%d) Altitud: %.2f km | Masa: %.2f kg | Velocidad: %.2f m/s | Periodo: %.2f min\n",
                   i + 1,
                   estado->historial[i].mision.altitud / 1000.0,
                   estado->historial[i].mision.masa_nave,
                   estado->historial[i].resultado.velocidad,
                   estado->historial[i].resultado.periodo_minutos);
        }
    }

    if (estado->hay_calculo_vigente) {
        printf("\nResultado correspondiente a los datos actuales:\n");
        printf("Velocidad orbital: %.2f m/s\n",
               estado->ultimo_calculo.velocidad);
        printf("Periodo orbital: %.2f minutos\n",
               estado->ultimo_calculo.periodo_minutos);
        printf("Aceleracion centripeta: %.4f m/s^2\n",
               estado->ultimo_calculo.aceleracion_centripeta);
        printf("Fuerza gravitatoria: %.2f N\n",
               estado->ultimo_calculo.fuerza_gravitatoria);
        printf("Fuerza centripeta: %.2f N\n",
               estado->ultimo_calculo.fuerza_centripeta);
    } else {
        printf("No hay un calculo orbital vigente para la configuracion actual.\n");
        if (estado->cantidad_resultados > 0) {
            printf("El historial conserva resultados de calculos anteriores.\n");
        }
    }

    if (estado->hay_pulso) {
        printf("\nUltimo pulso simulado:\n");
        printf("Desplazamiento lateral: %.2f m\n",
               estado->ultimo_pulso.desviacion);
        printf("Velocidad lateral: %.2f m/s\n",
               estado->ultimo_pulso.velocidad_lateral);
    } else {
        printf("No se ha simulado un pulso lateral.\n");
    }
    printf("===========================================\n");
}
