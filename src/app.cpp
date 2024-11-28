#include <Arduino.h>
#include <Wire.h>
#include <BluetoothSerial.h>
#include "comandos.h"
#include <LiquidCrystal.h>
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5
#define FREC_MIN 10600
#define FREC_MAX 11800

const int potencias[5] = {1,10,100,1000,10000};
int cifra_actual = 1;
int frecuencia = FREC_MIN;
bool mostrar = true;
long cnt = millis() + 1000;
static struct {
  int codigo; // 4 bits
  int ratio; 
} constexpr DIVISOR_REFERENCIA[]={
  {0b0000,2},
  {0b0001,4},
  {0b0010,8},
  {0b0011,16},
  {0b0100,32},
  {0b0101,64},
  {0b0110,128},
  {0b0111,256},
  {0b1000,24},
  {0b1001,5},
  {0b1010,10},
  {0b1011,20},
  {0b1100,40},
  {0b1101,80},
  {0b1110,160},
  {0b1111,320}
};

enum DIVISORES : int {
  DIV2,DIV4,DIV8,DIV16,DIV32,DIV64,DIV128,DIV256,DIV24,DIV5,DIV10,DIV20,DIV40,DIV80,DIV160,DIV320
};
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#define XTAL_F 4
#define R_DIV DIVISOR_REFERENCIA[DIV40].ratio // 64 //32
#define R_DIVr DIVISOR_REFERENCIA[DIV40].codigo
#define STATUS_POR 0b10000000
#define STATUS_PHASE_LOCK 0b01000000
#define botones 1

typedef enum i2cError{
  ENVIADO=0,
  muyLargo,
  nackAddres,
  nackData,
  ERROR,
  timeout  
}i2cError;
typedef enum modo{
  NORMAL,
  CP_SINK,
  CP_SOURCE,
  CP_DISABLE,
  COMPARADOR_FASE_TEST
}modo;

modo modoactual = NORMAL;
BluetoothSerial SerialBT;
const int zarlink = 0b1100001
; // Dirección I2C del Zarlink SP5769

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
  Wire.write(0b10000000 | R_DIVr); 
  Wire.write(0x00); 
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
      frecuencia = freq;
      int divisor = ((freq * R_DIV + XTAL_F*2)/(XTAL_F*4)) & 0x7FFF; //redondea para arriba
      if(modoactual ==COMPARADOR_FASE_TEST){
      SerialBT.println("Frecuencia");
      SerialBT.println(freq);
      SerialBT.println("Divisor");
      SerialBT.println(divisor);
      SerialBT.println("Frecuencia segun divisor y ref");
      SerialBT.println((XTAL_F*divisor*4*R_DIV/2)/R_DIV);}
      byte divH = (divisor >> 8) & 0x7F;
      byte divL = divisor & 0xFF;
      Wire.beginTransmission(zarlink);
      Wire.write(divH);
      Wire.write(divL);
      enviari2c(Wire.endTransmission());
    }
    static void I2C_write_mode(int mode){
      switch (mode){
        case NORMAL:
          configureSynth();
        break;case CP_SINK:
          Wire.beginTransmission(zarlink);
          Wire.write(0b11000000); 
          Wire.write(0x00); 
          enviari2c( Wire.endTransmission() );
          SerialBT.println("Charge PUMP --> SINK");
        break;case CP_SOURCE:
          Wire.beginTransmission(zarlink);
          Wire.write(0b11010000); 
          Wire.write(0x00); 
          enviari2c( Wire.endTransmission() );
          SerialBT.println("Charge PUMP --> SOURCE");
        break;case CP_DISABLE:
          Wire.beginTransmission(zarlink);
          Wire.write(0b11100000); 
          Wire.write(0x00); 
          enviari2c( Wire.endTransmission() );
          SerialBT.println("Charge PUMP --DISABLED--");
        break;case COMPARADOR_FASE_TEST:
          modoactual = COMPARADOR_FASE_TEST;
          Wire.beginTransmission(zarlink);
          Wire.write(0b11110000); 
          Wire.write(0x00); 
          enviari2c( Wire.endTransmission() );
          SerialBT.println("Salida del comparador de fase conectada a PIN ");
        break;}

      Wire.beginTransmission(zarlink);
      Wire.write(0);
      Wire.write(0);
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
  static const I2C i2c ={.write_freq=I2C_write_freq,.read_state=I2C_read_state,.write_mode=I2C_write_mode};
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
  modoactual = NORMAL;
  lcd.begin(16, 2); 
}


int read_LCD_buttons()  
  { adc_key_in = analogRead(0);      // Leemos A0
    // Mis botones dan:  0, 145, 329,507,743
    // Y ahora los comparamos con un margen comodo
    if (adc_key_in > 900) return btnNONE;     // Ningun boton pulsado 
    if (adc_key_in < 50)   return btnRIGHT; 
    if (adc_key_in < 250)  return btnUP;
    if (adc_key_in < 450)  return btnDOWN;
    if (adc_key_in < 650)  return btnLEFT;
    if (adc_key_in < 850)  return btnSELECT; 

    return btnNONE;  // Por si todo falla
  }
void loop() {
  if (SerialBT.available()){
    if(Comandos_procesa(static_cast<char>(SerialBT.read())))
        Serial.println("Comando procesado!");
  }
  lcd.setCursor(0,0);
  lcd.print("FRECUENCIA [MHZ]");
  lcd.setCursor(0,1);
  lcd.print(frecuencia);
  lcd.setCursor(cifra_actual,1);
  if (!mostrar) lcd.print('\0');
  if (cnt <= millis()){
     mostrar = !mostrar;
     cnt = millis() + 1000;}
  

  switch (read_LCD_buttons()){
    case btnRIGHT:      
      cifra_actual = (cifra_actual <  4) ? cifra_actual+1 : 0;
    break;case btnLEFT:
      cifra_actual = (cifra_actual > 0) ? cifra_actual-1 :  5;
    break;case btnDOWN:
      frecuencia = frecuencia  - potencias[cifra_actual] > FREC_MIN ? frecuencia - potencias[cifra_actual] : frecuencia;
    break;case btnUP:
      frecuencia = frecuencia  - potencias[cifra_actual] < FREC_MAX ? frecuencia - potencias[cifra_actual] : frecuencia;
    break;case btnSELECT:
      I2C_write_freq(frecuencia);
    break;case btnNONE://FALLTHRU:

    break;
  }

}
