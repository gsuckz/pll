#include <Arduino.h>
#include <BluetoothSerial.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include "comandos.h"
#include "sintetizador.h"

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
// Ultimas variables asociadas

// modo modoactual;
void setup()
{
    static const UART uart = {
        .write_string = UART_write_string, .write_numero = UART_write_numero, .write = UART_write};
    static const I2C i2c = {.write_freq = SintetizadorCambiaFrecuencia,
                            .read_state = SintetizadorLeeModo,
                            .write_mode = SintetizadorCambiaModo};
    // Inicia la comunicación serial para depuración
    Serial.begin(9600);
    // Inicia la comunicación Bluetooth
    SerialBT.begin("Sintetizador de Banda Ku"); // Nombre del dispositivo Bluetooth
    // Inicia la comunicación I2C
    Wire.begin();
    // Configuración inicial del sintetizador (ejemplo)
    SintetizadorInicializa();
    Comandos_init(&uart);
    comandos_i2c(&i2c);
    // modoactual = NORMAL;
    // lcd.begin(16, 2);
    // lcd.createChar(0, flecha);
}
char caracter_recibido;
void loop()
{                               // Codigo para lectura en consola.
    if (SerialBT.available()) {
        //SerialBT.println("Comando Recbido"); // Esto que hace ?
        caracter_recibido = SerialBT.read();
        //Serial.print("Se Recibio:");
        Serial.print(caracter_recibido);
        if (Comandos_procesa(static_cast<char>(caracter_recibido))){ 
            SerialBT.println("Comando procesado!");
            Serial.println("Comando procesado!");
            
        } 
    }
}
