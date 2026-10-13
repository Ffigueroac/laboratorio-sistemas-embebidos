const int PIN_BOTON = 32;

volatile unsigned long contadorISR = 0;
unsigned long contadorMostrado = 0;

void IRAM_ATTR detectarPulso() {
  contadorISR++;
}

void setup() {
  Serial.begin(115200);

  pinMode(PIN_BOTON, INPUT_PULLUP);

  attachInterrupt(
    digitalPinToInterrupt(PIN_BOTON),
    detectarPulso,
    FALLING
  );

  Serial.println("Prueba debounce por hardware");
}

void loop() {

  unsigned long valorActual;

  noInterrupts();
  valorActual = contadorISR;
  interrupts();

  if (valorActual != contadorMostrado) {

    contadorMostrado = valorActual;

    Serial.print("Conteo: ");
    Serial.println(contadorMostrado);
  }
}