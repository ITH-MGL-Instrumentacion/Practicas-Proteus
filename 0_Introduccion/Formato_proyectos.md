<h1>Formato de los proyectos en Visual Studio Code</h1>

- [Objetivo](#objetivo)
- [Estructura general del repositorio](#estructura-general-del-repositorio)
- [Flujo de trabajo recomendado](#flujo-de-trabajo-recomendado)
  - [1) Editar y validar código en VS Code](#1-editar-y-validar-código-en-vs-code)
  - [2) Probar la simulación en Proteus](#2-probar-la-simulación-en-proteus)
  - [3) Entregar la práctica](#3-entregar-la-práctica)
- [Extensiones recomendadas de VS Code](#extensiones-recomendadas-de-vs-code)
- [Prompt corto para errores de IntelliSense en `.ino`](#prompt-corto-para-errores-de-intellisense-en-ino)

## Objetivo

En este curso, el **código se escribe principalmente en Visual Studio Code** para facilitar la programación y detectar errores de forma temprana (IntelliSense, autocompletado y revisión de sintaxis).  
Después, ese código se prueba en la simulación de **Proteus**.

## Estructura general del repositorio

El repositorio está dividido por prácticas (salida digital, entrada digital, PWM, entrada analógica, etc.) y cada práctica incluye archivos de:

- **Arduino** (`.ino`)
- **Proteus** (`.pdsprj` y respaldos)

Esto permite separar claramente:

- La lógica del programa (Arduino)
- La simulación del circuito (Proteus)

## Flujo de trabajo recomendado

### 1) Editar y validar código en VS Code

1. Abre la carpeta del repositorio en VS Code.
2. Edita el archivo `.ino` de la práctica correspondiente.
3. Revisa errores de sintaxis e IntelliSense para corregir antes de simular.

### 2) Probar la simulación en Proteus

1. Abre el proyecto `.pdsprj` de la práctica.
2. Ejecuta la simulación y verifica que el comportamiento sea correcto.
3. Completa los ejercicios adicionales de cada práctica.

### 3) Entregar la práctica

Para la entrega, considera lo indicado en el índice principal:

- Un **commit** con los archivos modificados de Arduino y Proteus.
- Un **video corto** explicando la simulación completa.

## Extensiones recomendadas de VS Code

Este repositorio incluye recomendaciones de extensiones en `.vscode/extensions.json` y configuración base en `.vscode/settings.json`.

Para instalarlas:

- Abre la paleta de comandos (`Ctrl+Shift+P`)
- Ejecuta `Extensions: Show Recommended Extensions`
- Instala las recomendadas para el workspace

También puedes instalar extensiones por terminal con `code --install-extension <id-extension>`, aunque normalmente es más cómodo usar las recomendaciones del proyecto.

## Prompt corto para errores de IntelliSense en `.ino`

Si te aparecen errores de IntelliSense en archivos `.ino`, pídele a la IA de VS Code lo siguiente:

> Configura este proyecto para Arduino UNO en VS Code: usa Arduino Community Edition, ajusta IntelliSense para Arduino/C++, y valida que `#include <Arduino.h>` se resuelva correctamente.

> Nota: algunas rutas del core/toolchain dependen de cada equipo y pueden requerir ajuste local.

| Anterior | Índice | Siguiente |
|---|---|---|
| [Introducción a Proteus](Introduccion_Proteus.md) | [Volver al índice](../README.md#temas-de-introducción) | [Salida digital](../1_Salida_Digital/Salida_Digital.md) |
