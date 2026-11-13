# Informe técnico: Simulador Artemis LEO

## 1. Objetivo

El objetivo es desarrollar un programa en lenguaje C que permita estudiar la órbita terrestre baja de la nave Orion. El simulador calcula magnitudes de una órbita circular a partir de la ley de gravitación universal y permite estimar el desplazamiento lateral durante un pulso de aceleración constante.

## 2. Modelo físico

Se utilizan los datos de la consigna:

- Constante de gravitación universal: G = 6,674 × 10⁻¹¹ N·m²/kg².
- Masa de la Tierra: MT = 5,972 × 10²⁴ kg.
- Radio terrestre: RT = 6.371.000 m.
- Altitud inicial: h = 192.000 m.
- Masa de Orion: m = 33.000 kg.

El radio orbital se obtiene sumando el radio terrestre y la altitud:

r = RT + h = 6.371.000 m + 192.000 m = 6.563.000 m.

### 2.1. Relación entre fuerza gravitatoria y fuerza centrípeta

En una órbita circular ideal, la fuerza gravitatoria proporciona la fuerza centrípeta. Por lo tanto, sus módulos son iguales:

Fg = Fc

G · MT · m / r² = m · v² / r

Se simplifica la masa de Orion, que aparece en ambos miembros:

G · MT / r² = v² / r

Despejando la velocidad orbital:

v = √(G · MT / r)

Con los datos iniciales, la velocidad es aproximadamente 7.792,95 m/s. El programa calcula ambas fuerzas y permite comprobar que sus valores coinciden, con pequeñas diferencias posibles por el redondeo de la presentación.

### 2.2. Período orbital

El período es el tiempo que tarda la nave en completar una vuelta. La longitud de una circunferencia es 2πr. Al dividir esa longitud por la velocidad:

T = 2πr / v

T ≈ 5.291,52 s

Para expresar el resultado en minutos:

T(min) = 5.291,52 / 60 ≈ 88,19 min

### 2.3. Aceleración centrípeta

La aceleración centrípeta se calcula mediante:

ac = v² / r

Con los valores iniciales:

ac ≈ 9,2534 m/s²

### 2.4. Pulso lateral

Se considera que la nave parte con velocidad lateral inicial nula y recibe una aceleración lateral constante, perpendicular a su velocidad orbital inicial. Durante el intervalo del pulso, el desplazamiento lateral se calcula mediante:

d = ½ · a · (Δt)²

La velocidad lateral al finalizar el pulso es:

vlateral = a · Δt

Para a = 0,50 m/s² y Δt = 10,0 s:

- d = ½ · 0,50 · 10² = 25 m.
- vlateral = 0,50 · 10 = 5 m/s.

La maniobra agrega una componente de velocidad perpendicular a la velocidad orbital inicial. En el modelo simplificado, la componente lateral se analiza como un movimiento con aceleración constante. Esta aproximación permite calcular su desplazamiento y velocidad lateral, pero no representa por sí sola la trayectoria orbital completa. Por eso la nave se aparta de la órbita circular original. El programa calcula el desplazamiento lateral durante el pulso y la velocidad lateral alcanzada; no determina la nueva órbita completa posterior.

## 3. Mecánica de juego

El menú incorpora un desafío por cuatro turnos, en el que la tripulación debe tomar decisiones relacionadas con la inserción orbital y la estabilidad de Orion:

1. **Inserción orbital:** elegir la expresión correcta para calcular la velocidad orbital. La respuesta correcta suma 25 puntos; una incorrecta resta 15 puntos de integridad.
2. **Evaluación de la órbita:** decidir si la gravedad proporciona la fuerza centrípeta o si deben mantenerse los motores encendidos continuamente. La decisión correcta suma 25 puntos; la incorrecta resta 20 puntos de integridad.
3. **Maniobra lateral:** ejecutar el pulso recomendado (0,50 m/s² durante 10 s) o ingresar valores propios. Una desviación entre 0 y 25 m inclusive suma 30 puntos; superar ese límite resta 30 puntos de integridad.
4. **Respuesta operativa:** observar la desviación calculada y decidir si se mantiene el plan (desviación de hasta 25 m) o se cancelan nuevas maniobras laterales y se activa contingencia (desviación superior a 25 m). El programa informa el umbral, pero no muestra la respuesta recomendada antes de que el jugador decida. La decisión adecuada suma 20 puntos; la inadecuada resta 20 puntos de integridad.

La integridad comienza en 100 y no puede quedar por debajo de cero. El reporte final muestra el puntaje sobre 100, la integridad operativa, la cantidad de errores, las cuatro decisiones tomadas y los resultados físicos de la misión. La misión se considera exitosa si se obtienen al menos 70 puntos y la integridad final es mayor que cero.

Los límites de 25 m, las penalizaciones y el puntaje mínimo son reglas lúdicas definidas para el juego, no límites físicos adicionales de la consigna. El modelo físico del pulso sigue siendo el cálculo simplificado de desplazamiento lateral durante la aceleración constante.

## 4. Diseño del programa

El programa se divide en tres módulos:

- `main.c`: contiene la función principal, controla la navegación del menú y deriva cada opción al módulo de simulación. La presentación del menú se realiza mediante `mostrarMenu()` en `artemis.c`.
- `artemis.h` y `artemis.c`: declaran e implementan las estructuras, constantes, inicialización del estado, configuración de la misión, cálculos físicos, administración del historial y presentación de resultados.
- `validaciones.h` y `validaciones.c`: declaran e implementan las funciones que leen y validan números enteros y reales.

Se utilizan las estructuras `DatosMision`, `ResultadoOrbital`, `ResultadoPulso` y `RegistroOrbital` para agrupar datos relacionados. `RegistroOrbital` guarda la configuración de la misión junto con el resultado obtenido, de modo que cada elemento del historial pueda identificarse. La estructura `EstadoSimulador` reúne la configuración y los resultados que se mantienen mientras se ejecuta el programa. Las funciones reciben punteros a estructuras para consultar o modificar esos datos. Los cálculos escriben sus resultados mediante parámetros puntero, sin devolver estructuras completas; así se sigue la recomendación de los apuntes de CursoC-I sobre estructuras y funciones.

El historial es un arreglo estático de diez registros. Cuando se llena, los elementos se desplazan una posición para conservar los diez registros más recientes. El resumen muestra la altitud, la masa, la velocidad y el período de cada registro. No se utiliza reserva dinámica de memoria.

El módulo de validaciones incluye funciones para leer enteros, números reales y caracteres. La lectura de caracteres se utiliza para confirmar la salida del programa: `s` finaliza el simulador y `n` permite volver al menú.

## 5. Prueba de escritorio: caso principal

| Dato o resultado | Valor |
|---|---:|
| Altitud | 192.000 m |
| Masa de Orion | 33.000 kg |
| Radio orbital | 6.563.000 m |
| Velocidad orbital | 7.792,95 m/s |
| Período orbital | 5.291,52 s |
| Período orbital | 88,19 min |
| Aceleración centrípeta | 9,2534 m/s² |
| Fuerza gravitatoria | 305.362,10 N |
| Fuerza centrípeta | 305.362,10 N |
| Aceleración lateral | 0,50 m/s² |
| Duración del pulso | 10 s |
| Desplazamiento lateral | 25 m |
| Velocidad lateral | 5 m/s |

## 6. Pruebas de validación

| Prueba | Entrada | Comportamiento esperado |
|---|---|---|
| Opción válida | `2` | Realiza el cálculo orbital. |
| Opción fuera de rango | `8` | Rechaza el valor y vuelve a solicitar la opción. |
| Confirmación de salida | `x` | Rechaza el carácter y vuelve a solicitar `s` o `n`. |
| Cancelación de salida | `n` | Vuelve al menú principal. |
| Confirmación de salida | `s` | Finaliza el programa. |
| Texto en lugar de número | `hola` | Rechaza la entrada y vuelve a solicitar el dato. |
| Altitud igual a cero | `0` | Rechaza el valor. |
| Altitud negativa | `-100` | Rechaza el valor. |
| Masa igual a cero | `0` | Rechaza el valor. |
| Número seguido de letras | `12abc` | Rechaza la entrada completa. |
| Resumen con historial | Dos cálculos con configuraciones distintas | Muestra la altitud y la masa asociadas a cada resultado. |
| Línea demasiado larga | Más caracteres que el tamaño del búfer | Descarta la línea y vuelve a solicitar el dato. |
| Aceleración del ejemplo | `0.50` | Acepta el valor. |
| Duración del ejemplo | `10` | Acepta el valor. |

Las pruebas de funcionamiento deben repetirse en el equipo donde se realizará la demostración. La tabla describe las entradas y las respuestas previstas por las validaciones implementadas.

## 7. Decisiones y límites del modelo

- Se considera una órbita circular ideal y se utilizan los valores constantes indicados por la consigna.
- La masa de la nave interviene en el cálculo de las fuerzas, pero se simplifica al despejar la velocidad orbital.
- El pulso lateral se analiza como un movimiento con aceleración constante durante el intervalo indicado.
- No se modelan perturbaciones orbitales, resistencia atmosférica ni la trayectoria completa posterior al pulso.
- Los resultados se muestran redondeados; los cálculos internos utilizan variables de tipo `double`.
## 8. Prueba del juego

Se ejecutó el desafío completo desde la opción 5 del menú: se eligió la fórmula correcta, se reconoció que la gravedad proporciona la fuerza centrípeta, se utilizó el pulso recomendado y se decidió mantener el plan. El programa mostró los cuatro turnos, calculó 25 m de desplazamiento lateral, guardó el cálculo orbital en el historial y emitió un reporte de 100/100 puntos, 100/100 de integridad y resultado de desafío superado. También se compiló con `make` usando `-Wall -Wextra -std=c11`, sin errores ni advertencias.
