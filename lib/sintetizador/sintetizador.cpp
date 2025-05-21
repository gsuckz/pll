#include "sintetizador.h"
#include <Arduino.h>
#include <BluetoothSerial.h>
#include <Wire.h>
#include <ctype.h>
#include <stdint.h>
/* Macros */

#define FREC_MIN 10600 // ver si se mantiene
#define FREC_MAX 11800 // ver si se mantiene
#define XTAL_F 4
#define R_DIV DIVISOR_REFERENCIA[DIV40].ratio // 64 //32
#define R_DIVr DIVISOR_REFERENCIA[DIV40].codigo
#define STATUS_POR 0b10000000
#define STATUS_PHASE_LOCK 0b01000000

/* Variables públicas*/

static BluetoothSerial SerialBT;
boolean modoTest           = false; // Leer para entender...
enum TipoTest_e testActual = CP_snk;

/* Variables privadas*/
static int frecuencia;

static struct {
    int codigo; // 4 bits
    int ratio;
} constexpr DIVISOR_REFERENCIA[] = {
    {0b0000, 2},  {0b0001, 4}, {0b0010, 8},  {0b0011, 16}, {0b0100, 32}, {0b0101, 64}, {0b0110, 128}, {0b0111, 256},
    {0b1000, 24}, {0b1001, 5}, {0b1010, 10}, {0b1011, 20}, {0b1100, 40}, {0b1101, 80}, {0b1110, 160}, {0b1111, 320}};

enum DIVISORES : int {
    DIV2,
    DIV4,
    DIV8,
    DIV16,
    DIV32,
    DIV64,
    DIV128,
    DIV256,
    DIV24,
    DIV5,
    DIV10,
    DIV20,
    DIV40,
    DIV80,
    DIV160,
    DIV320
};

static modo modoactual                 = NORMAL;
const int zarlink                      = 0b1100001; // Dirección I2C del Zarlink SP5769
const char *nombresTests[tipoTest_MAX] = {[CP_snk] = "CP snk", [CP_src] = "CP src", [CP_dis] = "CP dis"};
/* Declaración de funciones privadas */

/* Implementación de Funciones privadas */

/* Implementación de funciones públicas*/

void SintetizadorInicializa(void)
{
    // Ejemplo de configuración inicial del Zarlink SP5769
    Wire.beginTransmission(zarlink);
    Wire.write(0b10000000 | R_DIVr);
    Wire.write(0x00);
    enviari2c(Wire.endTransmission());
    SerialBT.println("Sintetizador configurado - Indique Frecuencia");
}



void SintetizadorCambiaFrecuencia(int freq)
{
    frecuencia  = freq;
    int divisor = ((freq * R_DIV + XTAL_F * 2) / (XTAL_F * 4)) & 0x7FFF; // redondea para arriba
    if (modoactual == COMPARADOR_FASE_TEST) {
        SerialBT.println("Frecuencia");
        SerialBT.println(freq);
        SerialBT.println("Divisor");
        SerialBT.println(divisor);
        SerialBT.println("Frecuencia segun divisor y ref");
        SerialBT.println((XTAL_F * divisor * 4 * R_DIV / 2) / R_DIV);
    }
    byte divH = (divisor >> 8) & 0x7F;
    byte divL = divisor & 0xFF;
    Wire.beginTransmission(zarlink);
    Wire.write(divH);
    Wire.write(divL);
    enviari2c(Wire.endTransmission());
}
void SintetizadorCambiaModo(int mode)
{
    switch (mode) {
    case NORMAL:
        SintetizadorInicializa();
        break;
    case CP_SINK:
        Wire.beginTransmission(zarlink);
        Wire.write(0b11000000);
        Wire.write(0x00);
        enviari2c(Wire.endTransmission());
        SerialBT.println("Charge PUMP --> SINK");
        break;
    case CP_SOURCE:
        Wire.beginTransmission(zarlink);
        Wire.write(0b11010000);
        Wire.write(0x00);
        enviari2c(Wire.endTransmission());
        SerialBT.println("Charge PUMP --> SOURCE");
        break;
    case CP_DISABLE:
        Wire.beginTransmission(zarlink);
        Wire.write(0b11100000);
        Wire.write(0x00);
        enviari2c(Wire.endTransmission());
        SerialBT.println("Charge PUMP --DISABLED--");
        break;
    case COMPARADOR_FASE_TEST:
        modoactual = COMPARADOR_FASE_TEST;
        Wire.beginTransmission(zarlink);
        Wire.write(0b11110000);
        Wire.write(0x00);
        enviari2c(Wire.endTransmission());
        SerialBT.println("Salida del comparador de fase conectada a PIN ");
        break;
    }

    Wire.beginTransmission(zarlink);
    Wire.write(0);
    Wire.write(0);
    enviari2c(Wire.endTransmission());
}
int SintetizadorLeeModo()
{
    uint8_t temp;
    Wire.beginTransmission(zarlink);
    Wire.requestFrom(zarlink, 1);
    if (Wire.available()) {
        Wire.readBytes(&temp, 1);
        if ((STATUS_POR & temp)) {
            SerialBT.println("ERROR DE ALIMENTACION o NO SE AJUSTO UNA FRECUENCIA");
        } else if (STATUS_PHASE_LOCK & temp) {
            SerialBT.println("ENCLAVADO!");
            return 1;
        } else {
            SerialBT.println("No enclavado!");
        }
        return 0;
    }
    SerialBT.println("ERROR AL LEER ESTADO");
    return 0;
}

void enviari2c(uint8_t valor)
{
    i2cError error = (i2cError)valor;
    switch (error) {
    case ENVIADO:
        SerialBT.println("OK");
        // break;case muyLargo:
        //   SerialBT.println("MENSAJE DEMASIADO LARGO");
        // break;case nackAddres:
        //   SerialBT.println("ERROR DE DIRECCION");
        // break;case nackData:
        //   SerialBT.println("ERROR DE ENVIO DE DATOS");
        // break;case ERROR:
        //   SerialBT.println("ERROR DESCONOCIDO");
        // break;case timeout:
        //   SerialBT.println("TIMEOUT");
        break;
    default:
        SerialBT.println("ERROR DE COMUNICACION!");
        break;
    }
}

/*
Configura el Timer para el barrido de frecuecnias
*/
void configurarBarrido(uint8_t min, uint8_t max, uint8_t step){

}
