#include <Arduino.h>
#include <Wire.h>
#include <BluetoothSerial.h>
#include "comandos.h"

#define XTAL_F 4
#define R_DIV 32
#define STATUS_POR 0b10000000
#define STATUS_PHASE_LOCK 0b01000000

typedef enum i2cError{
  ENVIADO=0,
  muyLargo,
  nackAddres,
  nackData,
  ERROR,
  timeout  
}i2cError;

BluetoothSerial SerialBT;
const int zarlink = 0b1100010; // Dirección I2C del Zarlink SP5769

void enviari2c(uint8_t valor){
  i2cError error = (i2cError)valor;
  switch (error){
  case ENVIADO:  
    SerialBT.println("OK");  
  //break;case muyLargo:
  //  SerialBT.println("MENSAJE DEMASIADO LARGO");
  //break;case nackAddres:
  //  SerialBT.println("ERROR DE DIRECCION");
  //break;case nackData:
  //  SerialBT.println("ERROR DE ENVIO DE DATOS");
  //break;case ERROR:
  //  SerialBT.println("ERROR DESCONOCIDO");
  //break;case timeout:
  //  SerialBT.println("TIMEOUT");
  break;default:
    SerialBT.println("ERROR DE COMUNICACION!");
  break;
  }
}


void configureSynth() {
  // Ejemplo de configuración inicial del Zarlink SP5769
  Wire.beginTransmission(zarlink);
  Wire.write(0b10000000 | R_DIV); // Dirección de registro (ejemplo)
  Wire.write(0x00); // Valor de configuración (ejemplo)
  enviari2c( Wire.endTransmission() );
  SerialBT.println("Sintetizador configurado - Indique Frecuencia");
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
      int divisor = (freq * R_DIV + XTAL_F*2)/(XTAL_F*4); //redondea para arriba
      byte divH = (divisor >> 8) & 0x7F;
      byte divL = divisor & 0xFF;
      Wire.beginTransmission(zarlink);
      Wire.write(divH);
      Wire.write(divL);
      enviari2c(Wire.endTransmission());
    }
    static int I2C_read_state(){
      uint8_t temp;
      //Wire.beginTransmission(zarlink);    
      Wire.requestFrom(zarlink, 1);     
      if(Wire.available()){
        Wire.readBytes( &temp , 1);
        if((STATUS_POR & temp)){
          SerialBT.println("ERROR DE ALIMENTACION o NO SE AJUSTO UNA FRECUENCIA");
        }else if(STATUS_PHASE_LOCK & temp){
          SerialBT.println("ENCLAVADO!");
          return 1;
        }else{
          SerialBT.println("No enclavado!");}
        return 0;}
      SerialBT.println("ERROR AL LEER ESTADO");
    return 0;
    }
}

void setup() {
  static const UART uart ={.write_string=UART_write_string,.write_numero=UART_write_numero,.write=UART_write};
  static const I2C i2c ={.write_freq=I2C_write_freq,.read_state=I2C_read_state};
  // Inicia la comunicación serial para depuración
  Serial.begin(9600);
  // Inicia la comunicación Bluetooth
  SerialBT.begin("Sintetizador de Banda Ku"); // Nombre del dispositivo Bluetooth
  Serial.println("Bluetooth Iniciado. Esperando conexión...");
  // Inicia la comunicación I2C
  Wire.begin();
  // Configuración inicial del sintetizador (ejemplo)
  configureSynth();
  Comandos_init(&uart);
  comandos_i2c(&i2c);
}

void loop() {
  if (SerialBT.available()){
    if(Comandos_procesa(static_cast<char>(SerialBT.read())))
        Serial.println("Comando procesado!");
  }
}
