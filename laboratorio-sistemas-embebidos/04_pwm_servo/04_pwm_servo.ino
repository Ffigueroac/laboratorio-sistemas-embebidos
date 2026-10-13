#include <ESP32Servo.h>

// =====================================================
// LABORATORIO 1 - PUNTO 4
// 2 LDR + Servo + Serial Plotter optimizado
// =====================================================


// ===================== PINES ==========================

const int LDR_IZQ = 35;
const int LDR_DER = 34;

const int PIN_SERVO = 26;


// ====================== ADC ===========================

const float VCC = 3.3;
const int ADC_MAX = 4095;

// Resistencias fijas de los divisores
const float R_FIJA = 10000.0;   // 10 kOhm


// ===================== SERVO ==========================

Servo servo;

const int ANGULO_MIN = 0;
const int ANGULO_CENTRO = 90;
const int ANGULO_MAX = 180;

float anguloActual = 90.0;
float anguloObjetivo = 90.0;


// ===================== CONTROL ========================

// Si la diferencia de voltaje es menor a 50 mV,
// consideramos que los dos LDR están aproximadamente iguales
const float TOLERANCIA_V = 0.05;

// Cuánto cambia el ángulo según la diferencia de voltaje
const float GANANCIA = 40.0;

// Movimiento progresivo
const float PASO_SERVO = 3.0;

// Tiempo entre actualizaciones
const int TIEMPO_MOVIMIENTO = 20;


// =====================================================
// PROMEDIO ADC
// Reduce ruido en las lecturas
// =====================================================

int leerADCpromedio(int pin) {

  const int MUESTRAS = 8;

  long suma = 0;

  for (int i = 0; i < MUESTRAS; i++) {
    suma += analogRead(pin);
  }

  return suma / MUESTRAS;
}


// =====================================================
// SETUP
// =====================================================

void setup() {

  Serial.begin(115200);

  pinMode(LDR_IZQ, INPUT);
  pinMode(LDR_DER, INPUT);

  // Configuración del servo
  servo.setPeriodHertz(50);

  servo.attach(
    PIN_SERVO,
    500,
    2400
  );

  // Iniciar centrado
  anguloActual = ANGULO_CENTRO;
  anguloObjetivo = ANGULO_CENTRO;

  servo.write(ANGULO_CENTRO);

  delay(1000);
}


// =====================================================
// LOOP
// =====================================================

void loop() {

  // ==================================================
  // 1. LEER LOS DOS LDR
  // ==================================================

  int adcIzq = leerADCpromedio(LDR_IZQ);
  int adcDer = leerADCpromedio(LDR_DER);


  // ==================================================
  // 2. CONVERTIR ADC A VOLTAJE
  // ==================================================

  float voltIzq =
    (adcIzq * VCC) / ADC_MAX;

  float voltDer =
    (adcDer * VCC) / ADC_MAX;


  // ==================================================
  // 3. CALCULAR RESISTENCIAS DE LOS LDR
  // ==================================================

  float resistenciaIzq;
  float resistenciaDer;


  if (voltIzq > 0.001) {

    resistenciaIzq =
      R_FIJA * ((VCC / voltIzq) - 1.0);
  }

  else {

    // Evitar división por cero
    resistenciaIzq = 1000000.0;
  }


  if (voltDer > 0.001) {

    resistenciaDer =
      R_FIJA * ((VCC / voltDer) - 1.0);
  }

  else {

    resistenciaDer = 1000000.0;
  }


  // Pasar a kiloohmios
  float resistenciaIzq_k =
    resistenciaIzq / 1000.0;

  float resistenciaDer_k =
    resistenciaDer / 1000.0;


  // ==================================================
  // 4. DIFERENCIA DE VOLTAJE
  // ==================================================

  float diferencia =
    voltIzq - voltDer;


  // ==================================================
  // 5. CALCULAR ANGULO OBJETIVO
  // ==================================================

  // --------------------------
  // CENTRO
  // --------------------------

  if (fabs(diferencia) <= TOLERANCIA_V) {

    anguloObjetivo = ANGULO_CENTRO;
  }


  // --------------------------
  // MAS LUZ A LA IZQUIERDA
  // --------------------------

  else if (voltIzq > voltDer) {

    float desplazamiento =
      diferencia * GANANCIA;

    anguloObjetivo =
      ANGULO_CENTRO - desplazamiento;
  }


  // --------------------------
  // MAS LUZ A LA DERECHA
  // --------------------------

  else {

    float desplazamiento =
      (-diferencia) * GANANCIA;

    anguloObjetivo =
      ANGULO_CENTRO + desplazamiento;
  }


  // ==================================================
  // 6. LIMITAR ANGULO OBJETIVO
  // ==================================================

  anguloObjetivo =
    constrain(
      anguloObjetivo,
      ANGULO_MIN,
      ANGULO_MAX
    );


  // ==================================================
  // 7. MOVIMIENTO PROGRESIVO DEL SERVO
  // ==================================================

  if (anguloActual < anguloObjetivo) {

    anguloActual += PASO_SERVO;

    if (anguloActual > anguloObjetivo) {
      anguloActual = anguloObjetivo;
    }
  }


  else if (anguloActual > anguloObjetivo) {

    anguloActual -= PASO_SERVO;

    if (anguloActual < anguloObjetivo) {
      anguloActual = anguloObjetivo;
    }
  }


  // ==================================================
  // 8. MOVER SERVO
  // ==================================================

  servo.write((int)anguloActual);


  // ==================================================
  // 9. SERIAL PLOTTER OPTIMIZADO
  // ==================================================

  // Limitamos SOLO la visualización de las resistencias
  // para que una resistencia muy alta no destruya la escala.
  float rIzqPlot =
    constrain(resistenciaIzq_k, 0.0, 50.0);

  float rDerPlot =
    constrain(resistenciaDer_k, 0.0, 50.0);

  // Escalamos SOLO la visualización del ángulo.
  // El servo sigue trabajando realmente entre 0 y 180 grados.
  float anguloPlot =
    anguloActual / 10.0;


  Serial.print("R_Izq_kOhm:");
  Serial.print(rIzqPlot, 2);

  Serial.print("\t");

  Serial.print("R_Der_kOhm:");
  Serial.print(rDerPlot, 2);

  Serial.print("\t");

  Serial.print("Angulo_div10:");
  Serial.println(anguloPlot, 1);


  delay(TIEMPO_MOVIMIENTO);
}