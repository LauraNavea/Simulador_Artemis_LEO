# Guía para publicar y trabajar con Git/GitHub

Esta guía sirve para organizar el trabajo real del equipo. Los comandos siguientes son ejemplos: deben ejecutarse en el repositorio del grupo y reflejar cambios que efectivamente haya realizado cada integrante.

## 1. Crear el repositorio

1. Crear un repositorio público en GitHub o GitLab.
2. Agregar los archivos fuente (`.c` y `.h`), el `Makefile` y la documentación.
3. No subir archivos generados como `*.o` ni el ejecutable `simulador`.
4. Copiar la URL real del repositorio en el README.

## 2. Primer envío del proyecto

Desde la carpeta del proyecto, luego de inicializar el repositorio y configurar el remoto:

```bash
git init
git add main.c artemis.c artemis.h validaciones.c validaciones.h Makefile README.md INFORME_TECNICO.md GUIA_GIT.md GUIA_VIDEO.md AUDITORIA_FINAL.md
git commit -m "Estructura inicial del simulador Artemis"
git branch -M main
git remote add origin URL_REAL_DEL_REPOSITORIO
git push -u origin main
```

Reemplazar `URL_REAL_DEL_REPOSITORIO` por la dirección que entrega GitHub o GitLab. Si el repositorio ya existe y tiene commits, seguir las instrucciones de la plataforma en lugar de repetir `git init`.

## 3. Trabajo colaborativo

Cada integrante debe realizar y registrar sus propias tareas. Se recomienda dividir el trabajo, por ejemplo, entre cálculos físicos, validaciones, menú e interfaz, pruebas y documentación. Los commits deben describir cambios reales y pequeños.

Ejemplo de secuencia de trabajo para una modificación real:

```bash
git status
git add archivos_modificados
git commit -m "Describe el cambio realizado"
git push
```

Para trabajar con una rama:

```bash
git switch -c nombre-de-la-tarea
```

Después de realizar y probar los cambios:

```bash
git add archivos_modificados
git commit -m "Describe el cambio realizado"
git push -u origin nombre-de-la-tarea
```

Luego se puede abrir un Pull Request o Merge Request para revisar e integrar la modificación.

## 4. Verificación antes de entregar

```bash
git log --oneline --graph --all
git status
```

Comprobar que:

- el repositorio sea público;
- el historial muestre el desarrollo real y progresivo;
- los commits correspondan a los integrantes que realizaron cada tarea;
- el proyecto compile desde una copia limpia;
- estén presentes los archivos fuente y la documentación.

**Importante:** no inventar commits, fechas, autores ni tareas. Esta guía no crea un historial por sí sola; el equipo debe publicar su trabajo real.
