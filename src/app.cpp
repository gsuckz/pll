#include "comandos.h"
#include "driver/i2c.h"
#include "interfaz.h"
#include "sintetizador.h"
#include "teclado.h"
#include "ticker.h"
#include <Arduino.h>
#include <BluetoothSerial.h>
#include <LiquidCrystal.h>
#include <Wire.h>

#define botones 1

BluetoothSerial SerialBT;
void SintetizadorInicializa();
void enviari2c(uint8_t);
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
    TickType_t xLastWakeTime;
    xLastWakeTime        = xTaskGetTickCount();
    static int tiempo_ms = 100;
    while (1) {
        tiempo_ms = SintetizadorTick();                            // Llama a la función que actualiza el sintetizador
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(tiempo_ms)); // Espera el tiempo retornado por SintetizadorTick
    }
}
// const int rs= D1, en=D2, d4= D4, d5=D5, d6=D6, d7=D7;
const int rs = D7, en = D6, d4 = D5, d5 = D4, d6 = D2, d7 = D1;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); // Instancia LCD

int mostrarTecla = 0;

void vtareaMostrarFrecuencia(void *pvParameters)
{
    static int borrar, contadorBorrar = 0;
    static char buffer[17]; // 16 + null
    int frecuencia = sintetizadorFrecuencia();
    int estado     = sintetizadorEstado();
    for (;;) {
        if (contadorBorrar++ > 3) {
            contadorBorrar = 0;
            borrar         = !borrar;
        }
        frecuencia = sintetizadorFrecuencia();
        estado     = sintetizadorEstado();
        lcd.clear();
        lcd.setCursor(0, 0);
        snprintf(buffer, sizeof(buffer), "Frec: %d MHz", frecuencia);
        lcd.print(buffer);
        if (borrar && sintetizadorEdicion()) {
            lcd.setCursor(11 - sintetizadorEdicion(), 0);
            lcd.print(" ");
        }
        lcd.setCursor(0, 1);
        if (estado) {
            lcd.print("Enclavado!");
        } else {
            lcd.print("No enclavado");
        }
        // lcd.print(mostrarTecla);
        // lcd.print(sintetizadorEdicion());
        // if (borrar) lcd.print("X");
        vTaskDelay(pdMS_TO_TICKS(200)); // Actualiza cada 1 segundo
    }
}

void vtareaComandosTeclado(void *pvParameters)
{
    EventGroupHandle_t eventGroup = static_cast<EventGroupHandle_t>(pvParameters);
    EventBits_t uxBits;
    for (;;) {
        // Espera por gestos de teclado (KEV_...)
        uxBits = xEventGroupWaitBits(eventGroup, KEV_ALL_DOWN | KEV_ALL_UP, pdTRUE, pdFALSE, portMAX_DELAY);

        if (uxBits & KEV_SELECT_DOWN) {
            sintetizadorTeclado(SELECT_KEY);
            mostrarTecla = SELECT_KEY;
        } else if (uxBits & KEV_LEFT_DOWN) {
            sintetizadorTeclado(LEFT_KEY);
            mostrarTecla = LEFT_KEY;
        } else if (uxBits & KEV_DOWN_DOWN) {
            sintetizadorTeclado(DOWN_KEY);
            mostrarTecla = DOWN_KEY;
        } else if (uxBits & KEV_UP_DOWN) {
            sintetizadorTeclado(UP_KEY);
            mostrarTecla = UP_KEY;
        } else if (uxBits & KEV_RIGHT_DOWN) {
            sintetizadorTeclado(RIGHT_KEY);
            mostrarTecla = RIGHT_KEY;
        } else {
            mostrarTecla = 0;
        }
    }
}

TaskHandle_t tareaPeriodicaHandle = NULL;
TecladoTaskParams tecladoConfig;
EventGroupHandle_t eventGroup;
void setup()
{
    pinMode(12, OUTPUT); // Configura el pin 2 como salida para el LED
    pinMode(13, OUTPUT); // Configura el pin 2 como salida para el LED
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);
    pinMode(A0, INPUT); // Configura el pin 2
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
    lcd.begin(16, 2);
    Comandos_init(&uart);
    comandos_i2c(&i2c);
    if (xTaskCreate(vTareaPeriodica, "TareaPeriodica", 4096, NULL, 3, &tareaPeriodicaHandle) != pdPASS) {
        digitalWrite(2, HIGH);
    }

    xTaskCreatePinnedToCore(vtareaMostrarFrecuencia,         // función de la tarea
                            "TareaMostrarFrec",              // nombre
                            configMINIMAL_STACK_SIZE + 1000, // stack
                            NULL,                            // parámetros
                            2,                               // prioridad
                            NULL,                            // handle
                            1                                // core (opcional, ESP32)
    );

    eventGroup = xEventGroupCreate();
    while (!eventGroup)
        digitalWrite(12, HIGH);
    tecladoConfig = {eventGroup, 25, A0};

    xTaskCreatePinnedToCore(vtareaComandosTeclado,          // función de la tarea
                            "DetectarTeclas",               // nombre
                            TECLADO_TASK_STACK_SIZE + 1000, // stack
                            eventGroup,                     // parámetros
                            1,                              // prioridad
                            NULL,                           // handle
                            1                               // core (opcional, ESP32)
    );

    if (xTaskCreatePinnedToCore(tecladoTask,                    // función de la tarea
                                "TareaTeclado",                 // nombre
                                TECLADO_TASK_STACK_SIZE + 1024, // stack
                                &tecladoConfig,                 // parámetros
                                2,                              // prioridad
                                NULL,                           // handle
                                1                               // core (opcional, ESP32)
                                ) == pdPASS)
    {
        // digitalWrite(2, LOW); // Enciende el LED si la tarea no se creó correctamente
    }
    digitalWrite(12, LOW);
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
