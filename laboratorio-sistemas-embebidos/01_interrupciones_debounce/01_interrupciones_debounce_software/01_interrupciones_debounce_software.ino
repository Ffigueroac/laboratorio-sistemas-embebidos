const int PIN_BOTON = 27;

volatile bool interrupcionDetectada = false;
volatile unsigned long tiempoInterrupcion = 0;

unsigned long contador = 0;

// Estado del sistema
bool botonBloqueado = false;

// 50 ms = 50 000 us
const unsigned long TIEMPO_DEBOUNCE = 50000;


// =====================================================
// INTERRUPCION
// =====================================================
void IRAM_ATTR detectarPulso() {

  unsigned long ahora = micros();

  // Solo registramos que ocurrio el FALLING
  interrupcionDetectada = true;
  tiempoInterrupcion = ahora;
}


// =====================================================
// SETUP
// =====================================================
void setup() {

  Serial.begin(115200);

  pinMode(PIN_BOTON, INPUT_PULLUP);

  attachInterrupt(
    digitalPinToInterrupt(PIN_BOTON),
    detectarPulso,
    FALLING
  );

  Serial.println("Debounce software: tiempo + estado");
}


// =====================================================
// LOOP
// =====================================================
void loop() {

  static unsigned long ultimoPulsoAceptado = 0;


  // -------------------------------------------------
  // 1. DETECTAR NUEVA PULSACION
  // -------------------------------------------------
  if (interrupcionDetectada && !botonBloqueado) {

    noInterrupts();

    bool evento = interrupcionDetectada;
    unsigned long tiempoEvento = tiempoInterrupcion;

    interrupcionDetectada = false;

    interrupts();


    if (evento) {

      // Condicion 1:
      // debe haber pasado el tiempo de debounce
      if (ultimoPulsoAceptado == 0 ||
          (tiempoEvento - ultimoPulsoAceptado) >= TIEMPO_DEBOUNCE) {

        // Condicion 2:
        // verificar que realmente esta presionado
        if (digitalRead(PIN_BOTON) == LOW) {

          contador++;

          ultimoPulsoAceptado = tiempoEvento;

          botonBloqueado = true;

          Serial.print("Conteo: ");
          Serial.println(contador);
        }
      }
    }
  }


  // -------------------------------------------------
  // 2. ESPERAR A QUE EL BOTON SEA SOLTADO
  // -------------------------------------------------
  if (botonBloqueado) {

    if (digitalRead(PIN_BOTON) == HIGH) {

      // Comprobamos que el HIGH permanezca estable
      delay(20);

      if (digitalRead(PIN_BOTON) == HIGH) {

        botonBloqueado = false;

        // Descartar cualquier interrupcion vieja
        interrupcionDetectada = false;
      }
    }
  }
}