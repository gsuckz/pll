#include <Arduino.h>
#include <Wire.h>
#include <BluetoothSerial.h>
#include "comandos.h"

#define XTAL_F 4
#define R_DIV 64

BluetoothSerial SerialBT;
const int zarlink = 0x58; // Dirección I2C del Zarlink SP5769


void configureSynth() {
  // Ejemplo de configuración inicial del Zarlink SP5769
  Wire.beginTransmission(zarlink);
  Wire.write(0x00); // Dirección de registro (ejemplo)
  Wire.write(0x00); // Valor de configuración (ejemplo)
  Wire.endTransmission();
  Serial.println("Sintetizador configurado");
}


extern "C"{
    static void UART_write_string(const char *str){
        SerialBT.print(str);
    }
    static void UART_write_numero(int num){
        SerialBT.print(num);
    }
    static void UART_write(int c){
        SerialBT.write(static_cast<uint8_t>(c));
    }
    static void I2C_write_freq(int freq){
        int div = freq * (R_DIV/(XTAL_F*4)); 
        byte divH = (div >> 8) & 0x7F;
        byte divL = div & 0xFF;
        Wire.beginTransmission(zarlink);
        Wire.write(divH);
        Wire.write(divL);
        Wire.endTransmission();
        Serial.println("Frecuencia configurada a " + String(freq) + " Hz");
    }
    static int I2C_read_freq(){
        Wire.beginTransmission(zarlink);
        return Wire.read();
    }
}

void setup() {
  static const UART uart ={.write_string=UART_write_string,.write_numero=UART_write_numero,.write=UART_write};
  // Inicia la comunicación serial para depuración
  Serial.begin(9600);
  // Inicia la comunicación Bluetooth
  SerialBT.begin("ESP32_BT"); // Nombre del dispositivo Bluetooth
  Serial.println("Bluetooth Iniciado. Esperando conexión...");
  // Inicia la comunicación I2C
  Wire.begin();
  // Configuración inicial del sintetizador (ejemplo)
  configureSynth();
  Comandos_init(&uart);
}

void loop() {
  // Si hay datos disponibles en la conexión Bluetooth

  if (SerialBT.available()){
    if(Comandos_procesa(static_cast<char>(SerialBT.read())))
        Serial.println("Comando procesado!");
  }
}
