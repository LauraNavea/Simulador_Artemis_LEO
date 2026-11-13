# Auditoría de cumplimiento — Simulador Artemis LEO

## Alcance

Se revisaron los archivos fuente, la documentación del proyecto y los materiales de C incluidos en `Guía de material para aprender C-20261030.zip`. Se corrigió el acceso al juego y se amplió el desafío a cuatro turnos con consecuencias sobre la integridad operativa, decisiones basadas en los resultados físicos y un reporte detallado.

## Resultado por requisito

| Requisito | Estado | Observación |
|---|---|---|
| Lenguaje C y ejecución por consola | Cumple | El proyecto se compila como programa de consola con GCC. |
| Menú principal | Cumple tras corrección | Se agregó al menú visible la opción 5 para iniciar el desafío. |
| Juego por turnos/eventos | Cumple | El desafío tiene cuatro turnos, decisiones, puntaje, integridad operativa y reporte final. |
| Reporte de desempeño | Cumple | Muestra puntaje, integridad final, errores, decisiones registradas y resultados físicos. |
| Cálculos orbitales | Cumple | Calcula velocidad, período en segundos/minutos, aceleración centrípeta y ambas fuerzas. |
| Relación entre gravedad y fuerza centrípeta | Cumple | Presenta la igualdad de fuerzas y el despeje de la velocidad. |
| Pulso lateral | Cumple con modelo simplificado | Calcula desplazamiento y velocidad lateral para aceleración constante durante el pulso. No calcula la órbita posterior completa. |
| Modularización `.c` / `.h` | Cumple | Se separan `main`, lógica Artemis y validaciones. |
| Arreglos y punteros | Cumple | Se usa un arreglo estático de historial y punteros a estructuras en las funciones. |
| Validación de entradas | Cumple en el código revisado | Rechaza texto inválido, caracteres sobrantes, entradas fuera de rango, líneas demasiado largas y caracteres de confirmación no permitidos. |
| Makefile | Cumple | Compila los tres módulos con `-Wall -Wextra -std=c11` y enlaza `-lm`. |
| README con escenario, reglas, funcionamiento y compilación | Cumple en contenido | Incluye diagrama, explicación, reglas del juego y comandos de compilación/ejecución. |
| Registro del uso de IA en README | Cumple en contenido | Describe los usos de ChatGPT y recuerda que el equipo debe verificar y comprender los cambios. |
| Repositorio público e historial progresivo | Pendiente de verificación | El ZIP no contiene un enlace real al repositorio ni el historial Git. El README conserva un texto para agregar la URL. |
| Commits de todos los integrantes | Pendiente externo | Deben existir commits reales y progresivos de cada integrante; no se pueden demostrar desde este ZIP. |
| Video de hasta cinco minutos | Pendiente externo | El ZIP contiene una guía de grabación, pero no el video ni su enlace. |
| Datos completos del equipo | Pendiente | En el README hay dos legajos indicados como `completar`. |


## Ajustes de la revisión final

- La desviación lateral de 0 m se considera dentro del intervalo operativo seguro de 0 a 25 m inclusive.
- En el cuarto turno se informa el umbral de 25 m, pero no se adelanta la respuesta correcta.
- Como el pulso ya fue ejecutado en el turno anterior, la decisión para desviaciones excesivas se expresa como cancelar nuevas maniobras laterales y activar contingencia.
- Se volvió a compilar el proyecto y se probaron el recorrido exitoso, el pulso nulo, el pulso excesivo y las entradas inválidas.

## Contenidos de C utilizados

El código revisado utiliza un subconjunto de los contenidos que aparecen en los materiales adjuntos:

- Tipos simples: `int`, `double` y `char`.
- Constantes simbólicas mediante `#define`.
- Operadores aritméticos, relacionales y lógicos.
- Condicionales `if` / `else` y selección `switch`.
- Ciclos `for`, `while` y `do-while`.
- Funciones, parámetros, valores de retorno y prototipos.
- Arreglos estáticos.
- Estructuras `struct` y definición de tipos con `typedef`.
- Punteros a estructuras y parámetros `const`.
- Entrada y salida estándar con `printf`, `fgets` y `getchar`.
- Conversión de texto con `strtol` y `strtod`.
- Funciones matemáticas de `math.h`, en particular `sqrt`.
- Compilación modular con archivos `.c`, `.h`, objetos y Makefile.

No se utiliza memoria dinámica, listas enlazadas, árboles, tablas hash, punteros a funciones ni técnicas avanzadas ajenas a lo necesario para el trabajo. Se quitó la dependencia de `errno`/`ERANGE` en las validaciones para mantener la implementación más próxima a las funciones de conversión mostradas en el material de C. Los límites de entrada del programa permiten rechazar los resultados de conversión que no se encuentran dentro de los rangos admitidos.

## Decisiones de juego

El desafío tiene cuatro turnos y asigna 25 puntos por la fórmula orbital correcta, 25 por reconocer que la gravedad proporciona la fuerza centrípeta, 30 por una desviación entre 0 y 25 m inclusive y 20 por elegir correctamente mantener el plan o cancelar nuevas maniobras laterales y activar contingencia según la desviación. Las respuestas incorrectas reducen la integridad operativa (15, 20, 30 o 20 puntos según el evento). El reporte registra las cuatro decisiones, los errores, el puntaje, la integridad y los resultados físicos. La misión se supera con al menos 70 puntos y una integridad final mayor que cero.

Los límites y penalizaciones son reglas lúdicas definidas por el equipo; no son valores impuestos por la consigna física.

## Alcance físico y advertencia

La órbita se modela como circular e ideal. El pulso se resuelve como un movimiento lateral de aceleración constante, con desplazamiento `d = 1/2 · a · (Δt)^2` y velocidad lateral `v = a · Δt`. Por lo tanto, los 25 m del caso de referencia representan el desplazamiento de esa componente durante los 10 segundos del pulso, no una integración completa de la trayectoria orbital perturbada ni el cálculo de la nueva órbita. Esta limitación está explicada en el README y en el informe técnico.

## Pruebas realizadas en esta revisión

- Compilación con `make`, `-Wall -Wextra -std=c11` y enlace con `-lm`: correcta, sin advertencias.
- Ejecución del juego completo desde la opción 5 del menú: correcta.
- Caso exitoso con fórmula correcta, evaluación correcta de las fuerzas, pulso recomendado y decisión de mantener el plan: 100/100 puntos e integridad 100/100.
- Verificación de que el cálculo orbital del juego se incorpora al historial: correcta.
- Fórmula incorrecta: no suma los 25 puntos del primer turno, reduce la integridad y el juego continúa.
- Pulso con desviación superior a 25 m: no suma los 30 puntos del turno y reduce la integridad; la decisión de cancelar nuevas maniobras laterales y activar contingencia se evalúa correctamente en el turno siguiente.
- Entradas `nan`, `inf`, `12abc` y opción de menú fuera de rango: rechazadas.
- Entrada decimal con coma: aceptada.
- Desviación lateral igual a cero: se considera dentro del umbral operativo.
- Turno final: el umbral se muestra sin adelantar la decisión correcta.
- Confirmación de salida inválida: rechazada.
- Fin de entrada estándar: el programa informa y finaliza.

## Conclusión

El código y la documentación incluidos en el ZIP corregido satisfacen los requisitos técnicos y funcionales que pueden comprobarse localmente, con las limitaciones físicas expresadas arriba. No puede afirmarse que la entrega completa cumpla al 100% hasta publicar el repositorio público con historial real, completar los datos faltantes del equipo y entregar el video solicitado.
