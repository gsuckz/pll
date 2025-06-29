#include "comandos.h"
#include "driver/i2c.h"
#include "sintetizador.h"
#include "ticker.h"
#include <Arduino.h>
#include <BluetoothSerial.h>
#include <LiquidCrystal.h>
#include <Wire.h>

#define botones 1

/* modoTest
False: modo conexión(para Establecer una frecuencia )
True: modo Test (realizando algún Test)

modo de Test:
  Charge Pump sink
  Charge Pump source
  Charge Pump disable
*/

BluetoothSerial SerialBT;

// ======= Declaración de Funciones ===========
void SintetizadorInicializa();
void enviari2c(uint8_t);
// ======= Declaración de Funciones para detección de Teclas ===========
extern "C" {
static void UART_write_string(const char *str)
{
    SerialBT.print(str);
}
static void UART_write_numero(int num)
{
    SerialBT.print(num);
}
static void UART_write(int c)
{
    SerialBT.write(static_cast<uint8_t>(c));
}
}

void vTareaPeriodica(void *pvParameters)
{
    int paso    = 0;
    bool estado = false;
    while (1) {
        digitalWrite(2, HIGH);
        if (paso >= 5) { // Cambia el estado del LED cada 500 ms
            estado = !estado;
            paso   = 0;
        } else {
            ++paso;
        }
        // SerialBT.println("Tarea periódica ejecutada");
        // const int tiempo_ms = SintetizadorTick();
        const int tiempo_ms = 100;
        
        SintetizadorTick(); // Llama a la función que actualiza el sintetizador
        if (tiempo_ms / portTICK_PERIOD_MS < 1) {
            // Si el tiempo es menor a 1 tick, espera al menos 1 tick
            SerialBT.println("Tiempo menor a 1 tick, esperando 1 ms");
            digitalWrite(2, LOW);
            vTaskDelay(1);
        } else {
            // Espera el tiempo calculado
            digitalWrite(2, LOW);
            vTaskDelay(pdMS_TO_TICKS(tiempo_ms));
        }
        // vTaskDelay(tiempo_ms / portTICK_PERIOD_MS); // Espera el tiempoPaso antes de volver a ejecutar
    }
}

// Tarea para parpadear LED
void blinkTask(void *pvParameters)
{
    pinMode(2, OUTPUT);

    while (true) {
        digitalWrite(2, HIGH);          // Encender LED
        vTaskDelay(pdMS_TO_TICKS(500)); // Esperar 500 ms

        digitalWrite(2, LOW);           // Apagar LED
        vTaskDelay(pdMS_TO_TICKS(500)); // Esperar 500 ms
    }
}

TaskHandle_t tareaPeriodicaHandle = NULL;
void setup()
{
    static const UART uart = {
        .write_string = UART_write_string, .write_numero = UART_write_numero, .write = UART_write};
    static const I2C i2c = {.write_freq        = SintetizadorCambiaFrecuencia,
                            .read_state        = estadoDelSintetizador,
                            .write_mode        = SintetizadorCambiaModo,
                            .configurarBarrido = configurarBarrido,
                            .paraBarrido       = paraBarrido};

    // Inicia la comunicación serial para depuración
    Serial.begin(9600);
    // Inicia la comunicación Bluetooth
    SerialBT.begin("Sintetizador de Banda Ku"); // Nombre del dispositivo Bluetooth
    // Inicia la comunicación I2C
    Wire.begin();
    Wire.setClock(400000); // 400 kHz
    // Configuración inicial del sintetizador (ejemplo)
    SintetizadorInicializa();
    Comandos_init(&uart);
    comandos_i2c(&i2c);
    //  xTaskCreatePinnedToCore(
    //  blinkTask,         // Función de la tarea
    //  "Blink Task",      // Nombre de la tarea
    //  1000,              // Stack size en palabras (no bytes)
    //  NULL,              // Parámetro para la tarea
    //  1,                 // Prioridad
    //  NULL,              // Manejador (no lo usamos)
    //  1                  // Núcleo (0 o 1)
    //);
    pinMode(2, OUTPUT); // Configura el pin 2 como salida para el LED
    digitalWrite(2, LOW);
    if (xTaskCreate(vTareaPeriodica, "TareaPeriodica", 4096, NULL, 1, &tareaPeriodicaHandle) != pdPASS) {
        digitalWrite(2, HIGH);
    }
}
char caracter_recibido;
void loop()
{ // Codigo para lectura en consola.
    if (SerialBT.available()) {
        caracter_recibido = SerialBT.read();
        Serial.print(caracter_recibido);
        if (Comandos_procesa(static_cast<char>(caracter_recibido))) {
        }
    }
}
