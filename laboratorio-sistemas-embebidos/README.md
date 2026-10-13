# Laboratorio de Sistemas Embebidos

Prácticas con **Arduino** y **ESP32**: interrupciones, timers, conversión A/D y salidas PWM.
Cada carpeta contiene el código de un punto del laboratorio. Los sketches detectan la placa
(`#if defined(ESP32)`) y ajustan pines y resolución del ADC automáticamente.

## Contenido

| # | Tema | Carpeta |
|---|------|---------|
| 1 | Interrupciones + antirrebote (conteo de pulsos) | [`01_interrupciones_debounce`](01_interrupciones_debounce) |
| 2 | Timers por interrupciones (tiempo entre pulsos) | [`02_timers_interrupciones`](02_timers_interrupciones) |
| 3 | Entradas A/D: lectura de 2 LDR + LEDs de alarma | [`03_adc_ldr`](03_adc_ldr) |
| 4 | Salida PWM: servo según la dirección de la luz | [`04_pwm_servo`](04_pwm_servo) |

## Pines utilizados

| Elemento | ESP32 |
|----------|-------|
| Botón (a GND, `INPUT_PULLUP`) | GPIO4 |
| LDR izquierda | GPIO35 |
| LDR derecha | GPIO34 |
| LED izquierda | GPIO25 |
| LED derecha | GPIO26 |
| Servo (señal) | GPIO26 |


## Requisitos

- Arduino IDE 2.x o PlatformIO
- ESP32: paquete de placas *esp32 by Espressif* + librería **ESP32Servo** (punto 4)

## Uso

1. Abre el `.ino` de la carpeta del punto.
2. Selecciona la placa y el puerto.
3. Sube el código y abre el Monitor Serial a **115200 baudios** (o el Serial Plotter en los puntos 3 y 4).

## Autores

- Francisco Figueroa Cervera, Wladimir Julian Revelo Cadena, Jaime Santiago Ortiz Hernandez Barrera  — Universidad Nacional De colombia/ Instrumentacion Electronica
