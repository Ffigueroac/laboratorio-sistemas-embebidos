const int PIN_BOTON = 27;

volatile bool interrupcionDetectada = false;
volatile unsigned long tiempoInterrupcion = 0;

// Debounce
bool botonBloqueado = false;
const unsigned long TIEMPO_DEBOUNCE = 50000; // 50 ms

// Medición entre pulsos
unsigned long tiempoPulsoAnterior = 0;
unsigned long contador = 0;

bool primeraPulsacion = true;


// =====================================================
// INTERRUPCION
// =====================================================
void IRAM_ATTR detectarPulso() {

  tiempoInterrupcion = micros();
  interrupcionDetectada = true;
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

  Serial.println("Punto 2 - Tiempo entre pulsaciones");
}


// =====================================================
// LOOP
// =====================================================
void loop() {

  static unsigned long ultimoPulsoAceptado = 0;


  // ==================================================
  // DETECTAR PULSACION
  // ==================================================
  if (interrupcionDetectada && !botonBloqueado) {

    noInterrupts();

    bool evento = interrupcionDetectada;
    unsigned long tiempoActual = tiempoInterrupcion;

    interrupcionDetectada = false;

    interrupts();


    if (evento) {

      // ----------------------------------------------
      // CONDICION 1:
      // Debe haber pasado el tiempo de debounce
      // ----------------------------------------------
      if (ultimoPulsoAceptado == 0 ||
          (tiempoActual - ultimoPulsoAceptado)
          >= TIEMPO_DEBOUNCE) {

        // --------------------------------------------
        // CONDICION 2:
        // El boton realmente debe seguir presionado
        // --------------------------------------------
        if (digitalRead(PIN_BOTON) == LOW) {

          contador++;

          ultimoPulsoAceptado = tiempoActual;

          botonBloqueado = true;


          // ==========================================
          // PRIMERA PULSACION
          // ==========================================
          if (primeraPulsacion) {

            tiempoPulsoAnterior = tiempoActual;

            primeraPulsacion = false;

            Serial.println();
            Serial.print("Pulso ");
            Serial.println(contador);

            Serial.println(
              "Primera pulsacion: referencia de tiempo guardada"
            );
          }


          // ==========================================
          // SEGUNDA PULSACION EN ADELANTE
          // ==========================================
          else {

            unsigned long intervalo =
              tiempoActual - tiempoPulsoAnterior;

            // Guardamos este tiempo para la siguiente
            tiempoPulsoAnterior = tiempoActual;


            // ----------------------------------------
            // Mostrar resultados
            // ----------------------------------------
            Serial.println();

            Serial.print("Pulso ");
            Serial.println(contador);

            Serial.print("Tiempo entre pulsos: ");
            Serial.print(intervalo / 1000.0);
            Serial.println(" ms");

            Serial.print("Tiempo entre pulsos: ");
            Serial.print(intervalo / 1000000.0, 3);
            Serial.println(" s");
          }
        }
      }
    }
  }


  // ==================================================
  // ESPERAR A QUE EL BOTON SEA SOLTADO
  // ==================================================
  if (botonBloqueado) {

    if (digitalRead(PIN_BOTON) == HIGH) {

      delay(20);

      if (digitalRead(PIN_BOTON) == HIGH) {

        botonBloqueado = false;

        interrupcionDetectada = false;
      }
    }
  }
}