---
title: 'TP1: Control de Acceso'

---

# TP1: Control de Acceso

Este proyecto es un sistema de control de acceso diseñado para gestionar el acceso de personas a un área restringida.

## Descripción del Proyecto

El sistema de control de acceso está diseñado para permitir el acceso si se detecta el ingreso de una secuencia valida. El sistema incluye los siguientes componentes:

- **Placa NUCLEO-F401RE:** Para procesamiento y control.
- **Matriz Keypad 4x4:** Para capturar la secuencia ingresada. Se conecta a la placa mediante pines GPIO.
- **LED Verde:** Se enciende cuando hay acceso concedido.
- **LED Rojo:** Se enciende cuando hay acceso denegado.
- **LED Amarillo:** Utilizado principalmente para indicar que el sistema funciona correctamente.
- **PC:** Para visualización del estado de acceso mediante terminal serie. 
## Diagrama del Sistema

![Diagrama del Sistema](https://i.imgur.com/HDPCpfB.png)

## Características

- **Largo de secuencia:** La secuencia de ingreso esta compuesta por 4 dígitos numéricos.
- **Claves de acceso:** Se configuran mediante firmware utilizando #define.
- **Acceso concedido:** En caso de coincidir la secuencia ingresada con la clave guardada en memoria, se enciende el led verde por 3 segundos.
- **Acceso denegado:** De no coincidir la secuencia ingresada con la clave en memoria, se enciende el led rojo por 1 segundo.
- **Timeout:** Se genera un evento timeout cuando se tarde más de 5 segundos en ingresar un dígito. De ocurrir, se resetea la secuencia ingresada. La cuenta de timeout se refresca con cada tecla presionada.
- **Terminal serie:** Los estados de acceso (concedido, denegado, timeout) se mostrarán por terminal serie. Se incluye el estado propiamente dicho, el usuario y la clave ingresada, según corresponda.

## Plataformas utilizadas
Para el desarrollo de este prototipo se utilizó  Keil Studio Cloud, junto con las librerías de Mbed  provistas por ARM.

## Selección de keypad
Puede utilizarse cualquier keypad 4x4 que posea 8 pines, ya que se realiza un barrido de filas y se detecta la columna asociada a la tecla presionada. Como ejemplo se tiene el siguiente [keypad sugerido](https://articulo.mercadolibre.com.ar/MLA-621167817-teclado-botonera-16-4x4-matrix-keypad-keyboard-mcu-arduino-_JM#position=4&search_layout=stack&type=item&tracking_id=a9602585-798c-4b45-b847-0de492bfb22b).
## Uso

1. Ingresar la secuencia dígito a dígito.
2. Luego de presionar el cuarto dígito se anuncia el estado del acceso. A futuro se prevee agregar una tecla de "enter".
3. De ocurrir un timeout, ingresar nuevamente la secuencia.

### Estructura del Código

- `main.cpp`: Contiene el flujo principal del programa.
- `mis_funciones.cpp`: Funciones asociadas al control de acceso. Son usadas por el main.
- `mis_funciones.hpp`: Posee los #define que gobiernan el comportamiento del control de acceso, como así también los prototipos de las funciones de `mis_funciones.cpp`.

## Contribuciones

Las contribuciones son bienvenidas. Por favor, abre un issue o un pull request para discutir cualquier cambio.

## Licencia

Este proyecto está licenciado bajo la Licencia MIT - ver el archivo [LICENSE](LICENSE) para más detalles.
Este es un ejemplo de cómo incrustar un video en Markdown.

## Video de Demostración

Aquí tienes un video de demostración del proyecto:

<iframe width="560" height="315" src="https://www.youtube.com/watch?v=fxJk-AN7_-4" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

