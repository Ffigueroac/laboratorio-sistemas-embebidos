// =====================================================
// LABORATORIO 1 - PUNTO 3
// Lectura de dos LDR + Serial Plotter
// =====================================================

// Pines LDR
const int LDR_IZQ = 35;
const int LDR_DER = 34;

// Pines LEDs
const int LED_IZQ = 25;
const int LED_DER = 26;

// Datos del divisor
const float VCC = 3.3;
const float R_FIJA = 10000.0;
const int ADC_MAX = 4095;

// Tolerancia inicial
const int TOLERANCIA = 50;

void setup() {

  Serial.begin(115200);

  pinMode(LDR_IZQ, INPUT);
  pinMode(LDR_DER, INPUT);

  pinMode(LED_IZQ, OUTPUT);
  pinMode(LED_DER, OUTPUT);

  digitalWrite(LED_IZQ, LOW);
  digitalWrite(LED_DER, LOW);
}

void loop() {

  // ==================================================
  // 1. LEER ADC
  // ==================================================

  int adcIzq = analogRead(LDR_IZQ);
  int adcDer = analogRead(LDR_DER);


  // ==================================================
  // 2. CONVERTIR A VOLTAJE
  // ==================================================

  float voltIzq =
    (adcIzq * VCC) / ADC_MAX;

  float voltDer =
    (adcDer * VCC) / ADC_MAX;


  // ==================================================
  // 3. CALCULAR RESISTENCIA DE CADA LDR
  // ==================================================

  float resistenciaIzq;
  float resistenciaDer;

  if (voltIzq > 0.0) {
    resistenciaIzq =
      R_FIJA * ((VCC / voltIzq) - 1.0);
  }
  else {
    resistenciaIzq = INFINITY;
  }

  if (voltDer > 0.0) {
    resistenciaDer =
      R_FIJA * ((VCC / voltDer) - 1.0);
  }
  else {
    resistenciaDer = INFINITY;
  }


  // ==================================================
  // 4. COMPARAR LAS DOS LDR
  // ==================================================

  int error = adcIzq - adcDer;

  if (error > TOLERANCIA) {

    // Más luz a la izquierda
    digitalWrite(LED_IZQ, HIGH);
    digitalWrite(LED_DER, LOW);
  }

  else if (error < -TOLERANCIA) {

    // Más luz a la derecha
    digitalWrite(LED_IZQ, LOW);
    digitalWrite(LED_DER, HIGH);
  }

  else {

    // Luz aproximadamente centrada
    digitalWrite(LED_IZQ, LOW);
    digitalWrite(LED_DER, LOW);
  }


  // ==================================================
  // 5. SERIAL PLOTTER
  // ==================================================

  Serial.print("Izquierda:");
  Serial.print(adcIzq);

  Serial.print(",");

  Serial.print("Derecha:");
  Serial.println(adcDer);


  delay(100);
}