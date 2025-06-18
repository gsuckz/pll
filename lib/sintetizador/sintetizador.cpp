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
#define TAMANO_BUFFER_BARRIDO 200
#define NUMERO_PASOS_TOTAL 1201 // Número total de pasos en el barrido de frecuencias (De acuerdo al divisor del PLL)

/* Variables públicas*/
enum estadoPLL : int { ENCLAVADO, NO_ENCLAVADO, ERROR_ALIMENTACION };

static BluetoothSerial SerialBT;
boolean modoTest           = false; // Leer para entender...
enum TipoTest_e testActual = CP_snk;

/* Variables privadas*/
static int frecuencia         = 10600; // Frecuencia actual del sintetizador en MHz
static int pasoFrecuencia     = 1;
static int pasoTiempo         = 10;            // Duración cada paso del barrido en ms
static int frecuenciaBarridoMin = FREC_MIN;     // Frecuencia minima del barrido
static int frecuenciaBarridoMax = FREC_MAX;     // Frecuencia maxima del barrido
static estadoPLL estadoActual   = ERROR_ALIMENTACION; // Estado del PLL
static bool leerEstado          = false;        // Indica si se debe leer el estado del sintetizador
// static int tiempoPasoFrecuencia = 100;      // Tiempo en ms entre pasos de frecuencia en modo barrido
static bool barrer                         = 0;
static bool actualizarFrecuencia           = 1;
static byte bufferH[TAMANO_BUFFER_BARRIDO] = {0}; // Buffer para almacenar los datos del barrido
static byte bufferL[TAMANO_BUFFER_BARRIDO] = {0}; // Buffer para almacenar los datos del barrido
static int numeroPasos;
static int pasoActual = 0; // Variable estática para llevar el control del paso actual del barrido
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
const int zarlink                      = 0b1100001; // Dirección I2C del Zarlink SP5769 (Wire maneja el ultimo bit de acuerdo a la función que se llame)
const char *nombresTests[tipoTest_MAX] = {[CP_snk] = "CP snk", [CP_src] = "CP src", [CP_dis] = "CP dis"};

#define SDA_PIN 21  // ⚠️ Ajustar si usás otros pines
#define SCL_PIN 22

void configurarSintetizadorI2C() {
    static bool i2c_configurado = false;
    if (i2c_configurado) return;

    //i2c_config_t conf;
    //conf.mode = I2C_MODE_MASTER;
    //conf.sda_io_num = SDA_PIN;
    //conf.scl_io_num = SCL_PIN;
    //conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    //conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    //conf.master.clk_speed = 100000; // 100 kHz
//
    //i2c_param_config(I2C_NUM_0, &conf);
    //i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);

    //Wire.begin(SDA_PIN, SCL_PIN);
    //Wire.setClock(100000);

    i2c_configurado = true;
}




void SintetizadorInicializa(void)
{
    // Ejemplo de configuración inicial del Zarlink SP5769
    configurarSintetizadorI2C(); // Configura I2C si no se ha hecho antes
    Wire.beginTransmission(zarlink);
    Wire.write(0b10000000 | R_DIVr);
    Wire.write(0x00);
    enviari2c(Wire.endTransmission());
    // SerialBT.println("Sintetizador configurado - Indique Frecuencia");
}

void SintetizadorFijaFrecuencia(void)
{
    int divisor = ((frecuencia * R_DIV + XTAL_F * 2) / (XTAL_F * 4)) & 0x7FFF; // redondea para arriba
    byte divH   = (divisor >> 8) & 0x7F;
    byte divL   = divisor & 0xFF;
    Wire.beginTransmission(zarlink);
    Wire.write(divH);
    Wire.write(divL);
    enviari2c(Wire.endTransmission());
    SerialBT.print("Frecuencia ajustada en:");
    SerialBT.print(frecuencia); // Muestra la frecuencia ajustada
    SerialBT.print(" MHz, Divisor: ");  
    SerialBT.print(divisor); // Muestra el divisor calculado    
    SerialBT.print(" (H: ");    
    SerialBT.print(divH); // Muestra el byte alto del divisor   
    SerialBT.print(", L: ");    
    SerialBT.print(divL); // Muestra el byte bajo del divisor
    SerialBT.println(")\n");
}

void SintetizadorCambiaFrecuencia(int freq)
{
    frecuencia = freq;
    barrer     = 0; // Desactiva el barrido de frecuencias
    actualizarFrecuencia = 1; // Activa la actualización de frecuencia
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
}


int SintetizadorActualizaEstado()
{
    uint8_t temp;
    // actualizarFrecuencia = 0; // Desactiva la actualización de frecuencia al leer el estado
    Wire.beginTransmission(zarlink); //Donde cambia el último bit (R/W) de la dirección I2C???
    Wire.requestFrom(zarlink, 1);
    // actualizarFrecuencia = 1; // Reactiva la actualización de frecuencia después de leer el estado
    if (Wire.available()) {
        Wire.readBytes(&temp, 1);
        if ((STATUS_POR & temp)) {
            estadoActual = ERROR_ALIMENTACION; // Error de alimentación o no se ajustó una frecuencia
            // SerialBT.println("ERROR DE ALIMENTACION o NO SE AJUSTO UNA FRECUENCIA");
        } else if (STATUS_PHASE_LOCK & temp) {
            estadoActual = ENCLAVADO; // El sintetizador está enclavado
            // SerialBT.println("ENCLAVADO!");
            return 1;
        } else {
            estadoActual = NO_ENCLAVADO; // El sintetizador no está enclavado
            // SerialBT.println("No enclavado!");
        }
        return 0;
    }
    SerialBT.println("ERROR AL LEER ESTADO");
    return 0;
}

int estadoDelSintetizador()
{
    switch (estadoActual) {
    case ENCLAVADO:
        SerialBT.println("Estado: Enclavado!");
        break;
    case NO_ENCLAVADO:
        SerialBT.println("Estado: No Enclavado");
        break;
    case ERROR_ALIMENTACION:
        SerialBT.println("Estado: Error de Alimentación");
        break;
    default:
        SerialBT.println("Estado: Desconocido");
        break;
    }
    return estadoActual; // Retorna el estado actual del sintetizador
}

void  enviari2c(uint8_t valor)
{
    i2cError error = (i2cError)valor;
    switch (error) {
    case ENVIADO:
        SerialBT.println("OK");
        //  break;case muyLargo:
        //    SerialBT.println("MENSAJE DEMASIADO LARGO");
        //  break;case nackAddres:
        //    SerialBT.println("ERROR DE DIRECCION");
        //  break;case nackData:
        //    SerialBT.println("ERROR DE ENVIO DE DATOS");
        //  break;case ERROR:
        //    SerialBT.println("ERROR DESCONOCIDO");
        //  break;case timeout:
        //    SerialBT.println("TIMEOUT");
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
{   barrer = 0; // Desactiva el barrido de frecuencias
    int numeroPasosf, numeroPasosT = 0;
    int divisor          = 1;                    // Variable para almacenar el divisor calculado
    int temp             = frecuenciaBarridoMin; // Variable temporal para la frecuencia mínima
    frecuenciaBarridoMax = max;
    frecuenciaBarridoMin = min;
    numeroPasosf =
        (frecuenciaBarridoMax - frecuenciaBarridoMin) / FRECUENCIA_PASO_MIN; // Calcula el número de pasos de frecuencia
    numeroPasosT   = duracion / FRECUENCIA_PASO_MIN;                         // Calcula el número de pasos de tiempo
    numeroPasos    = (numeroPasosf > numeroPasosT)
                         ? numeroPasosf
                         : numeroPasosT; // Elige el mayor número de pasos entre frecuencia y tiempo
    pasoFrecuencia = ((frecuenciaBarridoMax - frecuenciaBarridoMin) / numeroPasos); // Calcula el paso de frecuencia
    pasoTiempo     = duracion / numeroPasos;                                        // Calcula el tiempo de paso en ms
    if (numeroPasos > TAMANO_BUFFER_BARRIDO / 2) {
        SerialBT.println("ERROR: Numero de pasos excede el tamaño del buffer de barrido");
        return; // Verifica que el número de pasos no exceda el tamaño del buffer
    }
    temp = frecuenciaBarridoMin; // Reinicia la frecuencia temporal al mínimo
    for (int i = 0; i < numeroPasos; i++) {
        divisor    = ((temp * R_DIV + XTAL_F * 2) / (XTAL_F * 4)) & 0x7FFF; // redondea para arriba
        bufferH[i] = (divisor >> 8) & 0x7F; // Llena el buffer con las frecuencias del barrido
        bufferL[i] = divisor & 0xFF;        // Llena el buffer con las frecuencias del barrido
        temp += pasoFrecuencia;             // Incrementa la sigueinte frecuencia a guardar en el paso definido
        //if (temp > frecuenciaBarridoMax) {
        //    temp = frecuenciaBarridoMin; // Reinicia la frecuencia temporal al mínimo si supera el máximo
        //}
    }
    SerialBT.print("Barrido configurado: ");
    SerialBT.print("Frecuencia Minima: ");
    SerialBT.print(frecuenciaBarridoMin);
    SerialBT.print(" MHz, Frecuencia Maxima: ");
    SerialBT.print(frecuenciaBarridoMax);
    SerialBT.print(" MHz, Paso de Frecuencia: ");
    SerialBT.print(pasoFrecuencia);
    SerialBT.print(" MHz, Duración del Barrido: ");
    SerialBT.print(duracion);
    pasoActual = 0; // Reinicia el paso actual del barrido
    barrer = true; // Activa el barrido de frecuencias
}

void paraBarrido(void)
{
    barrer = false; // Desactiva el barrido de frecuencias
    frecuencia = frecuenciaBarridoMin + pasoFrecuencia * pasoActual; // Fija la frecuencia actual al valor mínimo del barrido más el paso actual
    actualizarFrecuencia = true; // Activa la actualización de frecuencia
    // SintetizadorFijaFrecuencia(); // Asegura que la frecuencia actual se envíe al sintetizador
    SerialBT.println("Frecuencia barrido detenida, frecuencia actual fijada: ");
    SerialBT.println(frecuencia); // Muestra la frecuencia actual
}






void enviarPasoI2C() {
    //uint8_t byteH = bufferH[pasoActual];
    //uint8_t byteL = bufferL[pasoActual];
    //i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    //if (!cmd) return;
    //i2c_master_start(cmd);
    //i2c_master_write_byte(cmd, (zarlink << 1) | I2C_MASTER_WRITE, false); // sin ACK
    //i2c_master_write_byte(cmd, byteH, false);
    //i2c_master_write_byte(cmd, byteL, false);
    //i2c_master_stop(cmd);
    //// Lanza la transmisión SIN esperar
    //i2c_master_cmd_begin(I2C_NUM_0, cmd, 0); // timeout 0 → no bloquea
    //i2c_cmd_link_delete(cmd);
    //// Avanza el paso (cíclico)

    Wire.beginTransmission(zarlink);
    Wire.write(bufferH[pasoActual]);
    Wire.write(bufferL[pasoActual]);
    enviari2c(Wire.endTransmission());

}

void SintetizadorTickBarrido(void)
{
    if (barrer) {
        if (pasoActual < numeroPasos) {
            Wire.beginTransmission(zarlink);
            Wire.write(bufferH[pasoActual]); // Envía el byte alto de la frecuencia al sintetizador
            Wire.write(bufferL[pasoActual]); // Envía el byte bajo de la frecuencia al sintetizador
            enviari2c(Wire.endTransmission()); // Envía los datos al sintetizador y verifica errores
            pasoActual = (pasoActual + 1) % numeroPasos;
        } else {
            pasoActual = 0; // Reinicia el paso actual si se ha alcanzado el número total de pasos
        }
    }
    return; // Retorna el tiempo de paso en ms para el barrido
}

int SintetizadorTick(void)
{
    // Aquí puedes implementar la lógica de actualización periódica del sintetizador
    // Por ejemplo, podrías verificar el estado del sintetizador o actualizar la frecuencia
    // según sea necesario.
    SerialBT.println("SintetizadorTick llamado");
    switch (modoactual) {
    case NORMAL:
        // Lógica para el modo normal
        if (barrer) {
            SintetizadorTickBarrido(); // Llama a la función de barrido si está activo
            SerialBT.print("Barrido activo");
        }else{
            if(actualizarFrecuencia){
                pasoTiempo = 100; // Establece el tiempo de paso en ms para el modo normal
                SintetizadorFijaFrecuencia(); // Envia la frecuencia al sintetizador
                actualizarFrecuencia = false; // Desactiva la actualización de frecuencia
            }
            SerialBT.print("XD");
            
            SintetizadorActualizaEstado(); // Actualiza el estado del sintetizador
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
