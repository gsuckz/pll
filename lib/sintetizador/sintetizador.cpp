#include "sintetizador.h"
#include "driver/i2c.h"
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
#define FRECUENCIA_PASO_MIN 1 // Paso de frecuencia en MHz
#define TAMAÑO_BUFFER_BARRIDO 200

/* Variables públicas*/

static BluetoothSerial SerialBT;
boolean modoTest           = false; // Leer para entender...
enum TipoTest_e testActual = CP_snk;

/* Variables privadas*/
volatile int frecuencia = 10600; // Frecuencia actual del sintetizador en MHz
volatile int pasoFrecuencia     = 1;
volatile int pasoTiempo  = 1; // Duración cada paso del barrido en ms
static int frecuenciaBarridoMin = FREC_MIN; // Frecuencia minima del barrido
static int frecuenciaBarridoMax = FREC_MAX; // Frecuencia maxima del barrido
static estadoPLL estadoActual = NO_ENCLAVADO; // Estado del PLL
static bool leerEstado = false; // Indica si se debe leer el estado del sintetizador
//static int tiempoPasoFrecuencia = 100;      // Tiempo en ms entre pasos de frecuencia en modo barrido
static bool barrer              = 0;
static bool actualizarFrecuencia              = 1;
static byte bufferH[TAMAÑO_BUFFER_BARRIDO] = {0}; // Buffer para almacenar los datos del barrido
static byte bufferL[TAMAÑO_BUFFER_BARRIDO] = {0}; // Buffer para almacenar los datos del barrido
static int  numeroPasos;
static struct {
    int codigo; // 4 bits
    int ratio;
} constexpr DIVISOR_REFERENCIA[] = {
    {0b0000, 2},  {0b0001, 4}, {0b0010, 8},  {0b0011, 16}, {0b0100, 32}, {0b0101, 64}, {0b0110, 128}, {0b0111, 256},
    {0b1000, 24}, {0b1001, 5}, {0b1010, 10}, {0b1011, 20}, {0b1100, 40}, {0b1101, 80}, {0b1110, 160}, {0b1111, 320}};

enum estadoPLL : int {
    ENCLAVADO,
    NO_ENCLAVADO,
    ERROR_ALIMENTACION
};

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
    //SerialBT.println("Sintetizador configurado - Indique Frecuencia");
}

void SintetizadorFijaFrecuencia(void)
{
    int divisor = ((frecuencia * R_DIV + XTAL_F * 2) / (XTAL_F * 4)) & 0x7FFF; // redondea para arriba
    byte divH   = (divisor >> 8) & 0x7F;
    byte divL   = divisor & 0xFF;
    //Wire.beginTransmission(zarlink);
    Wire.write(divH);
    Wire.write(divL);
    enviari2c(Wire.endTransmission());
}

void SintetizadorCambiaFrecuencia(int freq)
{
    frecuencia = freq;
    barrer = 0; // Desactiva el barrido de frecuencias
    Wire.beginTransmission(zarlink);
    SintetizadorFijaFrecuencia();                                        // Envia la frecuencia al sintetizador
    //  ??? int divisor = ((freq * R_DIV + XTAL_F * 2) / (XTAL_F * 4)) & 0x7FFF; // redondea para arriba
    //  ??? if (modoactual == COMPARADOR_FASE_TEST) {
    //  ???     SerialBT.println("Frecuencia");
    //  ???     SerialBT.println(freq);
    //  ???     SerialBT.println("Divisor");
    //  ???     SerialBT.println(divisor);
    //  ???     SerialBT.println("Frecuencia segun divisor y ref");
    //  ???     SerialBT.println((XTAL_F * divisor * 4 * R_DIV / 2) / R_DIV);
    //  ??? }
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

    // ?????????? Wire.beginTransmission(zarlink);
    // ?????????? Wire.write(0);
    // ?????????? Wire.write(0);
    // ?????????? enviari2c(Wire.endTransmission());
}
int SintetizadorActualizaEstado(){
    uint8_t temp;
    //actualizarFrecuencia = 0; // Desactiva la actualización de frecuencia al leer el estado
    Wire.beginTransmission(zarlink);
    Wire.requestFrom(zarlink, 1);
    //actualizarFrecuencia = 1; // Reactiva la actualización de frecuencia después de leer el estado
    if (Wire.available()) {
        Wire.readBytes(&temp, 1);
        if ((STATUS_POR & temp)) {
            estadoActual = ERROR_ALIMENTACION; // Error de alimentación o no se ajustó una frecuencia
            //SerialBT.println("ERROR DE ALIMENTACION o NO SE AJUSTO UNA FRECUENCIA");
        } else if (STATUS_PHASE_LOCK & temp) {
            estadoActual = ENCLAVADO; // El sintetizador está enclavado
            //SerialBT.println("ENCLAVADO!");
            return 1;
        } else {
            estadoActual = NO_ENCLAVADO; // El sintetizador no está enclavado
            //SerialBT.println("No enclavado!");
        }
        return 0;
    }
    SerialBT.println("ERROR AL LEER ESTADO");
    return 0;
}

estadoPLL SintetizadorLeeEstado()
{
 switch (estadoActual){
    case ENCLAVADO:
        //SerialBT.println("Estado: Enclavado");
        break;
    case NO_ENCLAVADO:
        //SerialBT.println("Estado: No Enclavado");
        break;
    case ERROR_ALIMENTACION:
        //SerialBT.println("Estado: Error de Alimentación");
        break;
    default:
        //SerialBT.println("Estado: Desconocido");
        break;
    }
    return estadoActual; // Retorna el estado actual del sintetizador

}




void enviari2c(uint8_t valor)
{
    i2cError error = (i2cError)valor;
    switch (error) {
    case ENVIADO:
        //SerialBT.println("OK");
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
void configurarBarrido(int min, int max, int duracion)
{   int numeroPasosf,numeroPasosT = 0;
    int divisor = 1; // Variable para almacenar el divisor calculado
    int temp = frecuenciaBarridoMin; // Variable temporal para la frecuencia mínima
    frecuenciaBarridoMax = max;
    frecuenciaBarridoMin = min;
    numeroPasosf = (frecuenciaBarridoMax - frecuenciaBarridoMin) / FRECUENCIA_PASO_MIN; // Calcula el número de pasos de frecuencia
    numeroPasosT = duracion / FRECUENCIA_PASO_MIN; // Calcula el número de pasos de tiempo
    numeroPasos = (numeroPasosf > numeroPasosT) ? numeroPasosf : numeroPasosT; // Elige el mayor número de pasos entre frecuencia y tiempo
    pasoFrecuencia       = ((frecuenciaBarridoMax - frecuenciaBarridoMin) / numeroPasos); // Calcula el paso de frecuencia
    pasoTiempo = duracion / numeroPasos; // Calcula el tiempo de paso en ms
    if (numeroPasos > TAMAÑO_BUFFER_BARRIDO / 2) {
        SerialBT.println("ERROR: Numero de pasos excede el tamaño del buffer de barrido");
        return; // Verifica que el número de pasos no exceda el tamaño del buffer
    }
    temp = frecuenciaBarridoMin; // Reinicia la frecuencia temporal al mínimo
    for (int i = 0; i < numeroPasos; i ++) {        
        divisor = ((temp * R_DIV + XTAL_F * 2) / (XTAL_F * 4)) & 0x7FFF; // redondea para arriba
        bufferH[i] = (divisor >> 8) & 0x7F; // Llena el buffer con las frecuencias del barrido
        bufferL[i] = divisor & 0xFF; // Llena el buffer con las frecuencias del barrido
        temp += pasoFrecuencia; // Incrementa la frecuencia temporal en el paso definido
        if (temp > frecuenciaBarridoMax) {
            temp = frecuenciaBarridoMin; // Reinicia la frecuencia temporal al mínimo si supera el máximo
        }   


    }

    //enviari2c(Wire.endTransmission()); Ejecutar en SitentizadorTick
    SerialBT.print("Barrido configurado: ");
    SerialBT.print("Frecuencia Minima: ");
    SerialBT.print(frecuenciaBarridoMin);
    SerialBT.print(" MHz, Frecuencia Maxima: ");
    SerialBT.print(frecuenciaBarridoMax);   
    SerialBT.print(" MHz, Paso de Frecuencia: ");
    SerialBT.print(pasoFrecuencia); 
    SerialBT.print(" MHz, Duración del Barrido: ");
    SerialBT.print(duracion);
    barrer               = true; // Activa el barrido de frecuencias
}

void paraBarrido(void){
    barrer = false;               // Desactiva el barrido de frecuencias
    //SintetizadorFijaFrecuencia(); // Asegura que la frecuencia actual se envíe al sintetizador
    SerialBT.println("Frecuencia barrido detenida, frecuencia actual fijada: ");
    SerialBT.println(frecuencia); // Muestra la frecuencia actual
    }

int SintetizadorTick(void)
{
    // Aquí puedes implementar la lógica de actualización periódica del sintetizador
    // Por ejemplo, podrías verificar el estado del sintetizador o actualizar la frecuencia
    // según sea necesario.
    switch (modoactual) {
    case NORMAL:
        // Lógica para el modo normal
        if (barrer) {
            //frecuencia += pasoFrecuencia; // Incrementa la frecuencia en el paso definido
            //SerialBT.println(frecuencia); // Muestra la frecuencia actual en el monitor serial
            //SerialBT.println((int)millis()); // Muestra el tiempo actual en milisegundos
            //if(actualizarFrecuencia) SintetizadorFijaFrecuencia(); // Asegura que la frecuencia actual se envíe al sintetizador
            if (frecuencia > frecuenciaBarridoMax) {
                frecuencia = frecuenciaBarridoMin; // Reinicia la frecuencia al mínimo si supera el máximo
                Wire.beginTransmission(zarlink);
                Wire.write(buffer, numeroPasos); // Envía el buffer al sintetizador
                i2cError resultado = (i2cError)Wire.endTransmission(); // Finaliza la transmisión y obtiene el resultado
                //SerialBT.println("Reiniciando barrido de frecuencias");
            }else{
                SintetizadorActualizaEstado(); // Actualiza el estado del sintetizador
            }
        }
        // Enviar la nueva frecuencia al sintetizador I2C
        break;
    case CP_SINK:
        // Lógica para el modo CP_SINK
        break;
    case CP_SOURCE:
        break;
    case CP_DISABLE:
        // Lógica para el modo CP_DISABLE
        break;
    case COMPARADOR_FASE_TEST:
        // Lógica para el modo COMPARADOR_FASE_TEST
        break;
    }

    return pasoTiempo; // Retorna el tiempo de paso en ms para el barrido
}
