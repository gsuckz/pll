#include <Arduino.h>
#include <Wire.h>
#include <BluetoothSerial.h>
#include "comandos.h"

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

void setFrequency(int frequency) {
  // Función para configurar la frecuencia del SP5769
  byte freqHighByte = (frequency >> 8) & 0x7F;
  byte freqLowByte = frequency & 0xFF;

  Wire.beginTransmission(zarlink);
  Wire.write(zarlink); // Dirección de registro de frecuencia (ejemplo)
  Wire.write(freqHighByte);
  Wire.write(freqLowByte);
  Wire.endTransmission();

  Serial.println("Frecuencia configurada a " + String(frequency) + " Hz");
}

bool compareStrings(String& str1, String& str2) {
  if (str1.length() != str2.length()) {
    return false;  // Si las longitudes son diferentes, las cadenas no son iguales
  }

  for (int i = 0; i < str1.length(); i++) {
    if (str1[i] != str2[i]) {
      return false;  // Si algún carácter no coincide, las cadenas no son iguales
    }
  }

  return true;  // Las cadenas son iguales
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
