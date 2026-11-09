# Cola de impresión — Interfaz gráfica en C++

Reimplementación visual del proyecto de **colas FIFO** de la documentación original.

## Funciones
- Cola circular de 6 posiciones.
- Trabajos enviados aleatoriamente desde PC1 a PC5.
- Botón **Agregar trabajo**: encola al final sin desplazar trabajos anteriores.
- Botón **Imprimir primero**: atiende y retira el primer trabajo.
- Botón **Mostrar cola**: consulta los trabajos en orden FIFO.
- Mensajes de **cola LLENA** y **cola VACÍA**.
- **Reiniciar**: borra todos los trabajos para repetir la demostración.
- Diagrama visual en pantalla con seis casillas, contador y registro de operaciones.

## Cómo ejecutarlo

**Requisito:** instalar Qt Widgets (Qt 5 o Qt 6), CMake y un compilador C++17. Es un programa de escritorio, no funciona como programa de consola ni directamente en OnlineGDB.

### Opción fácil — Qt Creator (Windows / macOS / Linux)
1. Instala Qt y Qt Creator desde https://www.qt.io/download-qt-installer-oss
2. Abre `CMakeLists.txt` con Qt Creator.
3. Elige un kit de escritorio Qt 5 o Qt 6 y pulsa **Run** (▶).

### Terminal (con Qt y CMake instalados)
```sh
cmake -S . -B build
cmake --build build
```
Ejecuta `build/ColaImpresionGUI` (o el ejecutable generado en Windows; en macOS puede ser `build/ColaImpresionGUI.app`).

## Para la auditoría de avances
**Antes:** cola circular con menú de texto (documentación de septiembre de 2026).
**Después:** mismos algoritmos FIFO con interfaz Qt Widgets, representación visual de los 6 espacios, botones y registro de operaciones. Esta versión es una modificación nueva y debe comprobarse ejecutándola y capturando pantalla antes de marcarla como probada.
