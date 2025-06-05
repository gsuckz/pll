#include "comandos.h"
#include "buffer.h"
#include "numeros.h"
#include <Arduino.h>
#include <ctype.h>
#include <stdint.h>

#define N_COMANDOS 16
#define RANGO_MINIMO 100         // Rango minimo de frecuencias permitido en MHz
#define PASO_MINIMO_FERCUENCIA 1 // Paso minimo de frecuencia en MHz

#define MAX_N_PARAMETROS 3
int fmin_barrido              = 10600; // Frecuencia minima del barrido en MHz
int fmax_barrido              = 11800; // Frecuencia maxima del barridoen MHz
int frecuencia_actual = 10600; // Frecuencia actual en MHz
uint64_t tiempoPaso        = 1000;  // Variable para el tiempo de paso del timer (en uS)

hw_timer_t *timer = NULL; // Timer para el I2C
void IRAM_ATTR timerInterrupcion(); // Prototipo de la función de interrupción del timer

typedef enum Command {
    APAGAR,
    BARRER,
    COMP_FASE_TEST,
    CP_disabled,
    CP_sink,
    CP_source,
    ESTADOq,
    FREC,
    FRECUENCIA,
    FRECUENCIAq,
    FRECq, // Frecuencia?
    IDq,   // ID?
    INICIAR,
    RESET_CMD,
    STOP,
    DESCO = 255
} Command;

typedef enum Codigo { CodigoValido, none, SyntaxError, FaltanParametros, SobranParametros } Codigo;

typedef struct CMD {
    Command cmd;
    int parametro[9];
    Codigo code;
} CMD;

static char const *const tabla_cmd[N_COMANDOS] = {"apagar",  "barrer\n3", "cft",        "cpdis",       "cpsnk", "cpsrc",
                                                  "estado",  "frec\n1",   "frecuencia", "frecuencia?", "frec?", "id?",
                                                  "iniciar", "reset",     "stop",       "test"};

typedef enum Estado { INICIO, buscaCMD, buscaNUM, blank, ERROR } Estado;

typedef struct Palabra {
    int min;
    int max;
    int n;
} Palabra;

static const UART *uart;
static const I2C *i2c;

void Comandos_init(const UART *uart_)
{
    uart  = uart_;
    timer = timerBegin(1, 160, true);                       // Timer 0, divisor de reloj 160, cuenta hacia arriba, cada ciclo del timer es 1 microsegundo
    timerAttachInterrupt(timer, &timerInterrupcion, true); // Adjuntar la función de manejo de interrupción
}
void comandos_i2c(const I2C *i2c_)
{
    i2c = i2c_;
}


void IRAM_ATTR timerInterrupcion()
{
    //i2c->write_freq(frecuencia_actual);          // Enviar la frecuencia actual al I2C
    frecuencia_actual += PASO_MINIMO_FERCUENCIA; // Incrementar la frecuencia actual
    if (frecuencia_actual > fmax_barrido) {
        frecuencia_actual = fmin_barrido; // Reiniciar a la frecuencia minima si se supera la maxima
        //uart->write_string("Reiniciando barrido de frecuencias\n\r");
        //Serial.println("Reiniciando barrido de frecuencias");
    }
}

static void agregarLetra(Palabra *palabra, char c)
{
    if (palabra->max < palabra->min) {
        return; // Si no hubo ninguna coincidencia la ultima vez, no hace nada.
    }
    if (isalpha(c)) {
        c = tolower(c);
    }
    for (int i = palabra->min; i <= palabra->max; i++) { //  Busco la primera coincidencia.
        char caracter_tabla = tabla_cmd[i][palabra->n];
        if (c == caracter_tabla) {
            palabra->min = i;
            break;
        }
    }

    for (int i = palabra->min; i <= palabra->max; i++) { // Busco la primera diferencia.
        char caracter_tabla = tabla_cmd[i][palabra->n];
        if (c != caracter_tabla) {
            palabra->max = i - 1;
            break;
        }
    }
    palabra->n++;
}
static void palabraCLR(Palabra *palabra)
{
    palabra->max = N_COMANDOS - 1;
    palabra->min = 0;
    palabra->n   = 0;
}
static Command getCMD(Palabra *palabra)
{
    Command const comando = ((palabra->max >= palabra->min) && ((tabla_cmd[palabra->min][palabra->n] == '\0') || //comprueba que haya determinado una posicion y que haya leido un comando
                                                                (tabla_cmd[palabra->min][palabra->n] == '\n')))

                                ? palabra->min
                                : DESCO;
    return comando;
}
static uint8_t cmd_c_parametro(Palabra *palabra)
{
    uint8_t cantidadParametros = 0;
    if (tabla_cmd[palabra->min][palabra->n] == '\n') {
        cantidadParametros = (tabla_cmd[palabra->min][palabra->n + 1] - '0'); //Por como se cofican los numeros en ASCII, el caracter '0' es 48
    } else {
        cantidadParametros = 0;
    }
    return cantidadParametros;
}
static bool esTerminador(char c)
{
    return (c == '\n'); //|| c == '\r' (El primero que se envía es '\n' por lo que no se consulta el otro)
                        // no molesta despues??
}

// Paso minimo de frecuencia en MHz = 1 MHz
// Tiempo minimo de paso = ??
// Tiempo de paso en mmicrosegundos = Tiempo de Barrido / nro de pasos
// Numero de pasos = (fmax_barrido - fmin_barrido) / paso minimo de frecuencia
// Tiempo de paso = (fmax_barrido - fmin_barrido) / paso minimo de frecuencia

static void procesar_cmd(CMD *cmd)
{
    switch (cmd->code) {
    case SyntaxError:
        uart->write_string("Error de Syntaxis\r\n");
        return;
        break;
    case FaltanParametros:
        uart->write_string("Se necesitan mas parámetros\n\r");
        return;
        break;
    case SobranParametros:
        uart->write_string("Demasiados Parametros para el comando\n\r");
        return;
        break;
    default:
        break;
    }
    switch (cmd->cmd) {
    case BARRER:
    
        if (timer == NULL) {
            uart->write_string("Timer no inicializado\n\r");
            return;
        }
        if (cmd->parametro[0] < 1 || cmd->parametro[0] > 10000) {
            uart->write_string("Tiempo de barrido invalido, ingrese un valor entre 1 y 10000 ms\n\r");
            return;
        }
        if (cmd->parametro[1] < 10600 || cmd->parametro[1] > 11800 || cmd->parametro[2] < 10600 ||
            cmd->parametro[2] > 11800)
        {
            uart->write_string("Frecuencia fuera del rango permitido (10600-11800 MHz)\n\r");
            uart->write_numero(cmd->parametro[1]);
            uart->write_string(" MHz\n\r");     
            uart->write_numero(cmd->parametro[2]);
            uart->write_string(" MHz\n\r");
            return;
        }
        if (cmd->parametro[1] >= cmd->parametro[2]) {
            uart->write_string("Frecuencia minima debe ser menor que la maxima\n\r");
            return;
        }
        if ((cmd->parametro[2] - cmd->parametro[1]) < RANGO_MINIMO)
        { // RANGO_MINIMO es el rango minimo de frecuencias permitido
            uart->write_string("Rango de frecuencias demasiado pequeño\n\r");
            return;
        }

        fmin_barrido = cmd->parametro[1];
        fmax_barrido = cmd->parametro[2];
        tiempoPaso =
        (cmd->parametro[0] * 1000) /
        ((fmax_barrido - fmin_barrido)/PASO_MINIMO_FERCUENCIA); // Convertir a microsegundos, divide el tiempo de barrido por el numero de pasos
        uart->write_string("Barrido iniciado\n\r");
        uart->write_string("Frecuencia minima: ");
        uart->write_numero(fmin_barrido);       
        uart->write_string(" MHz\n\r");
        uart->write_string("Frecuencia maxima: ");  
        uart->write_numero(fmax_barrido);
        uart->write_string(" MHz\n\r"); 
        uart->write_string("Tiempo de barrido: ");
        uart->write_numero(cmd->parametro[0]);
        uart->write_string(" ms\n\r");
        timerAlarmDisable(timer); // Deshabilitar la alarma antes de configurarla
        timerAlarmWrite(timer, tiempoPaso, true); // Interrupción cada tiempoPaso microsegundos
        timerAlarmEnable(timer);
        //Serial.println("Iniciando barrido de frecuencias");
        break;
    case STOP:
        timerAlarmDisable(timer); // Deshabilitar la alarma 
        uart->write_string("Barrido detenido\n\r");
        //Serial.println("Barrido Detenido");
        break;
    case FREC:
    case FRECUENCIA:                                                    // FALLTHRU
        if (cmd->parametro[0] <= 11800 && cmd->parametro[0] >= 10600) { //&& (cmd->code = CodigoValido)
            // set_servo_angle(cmd->parametro[0]); Escribir en I2C valor del divisor de frecuencia
            i2c->write_freq(cmd->parametro[0]);
        } else {
            uart->write_string("Frecuencia invalida, ingrese un valor entero entre 10600-11800 MHz\n\r");
        }
        break;
    case FRECq:
    case FRECUENCIAq: // FALLTHRU
        uart->write_string("Frecuencia fijada en: ");
        uart->write_numero(frecuencia_actual);
        uart->write('\n');
        uart->write('\r');
        break;
    case IDq:
        uart->write_string("Sintetizador de Frecuencias en Banda Ku v0.1\n\r");
        break;
    case ESTADOq:
        i2c->read_state();
        break;
    case CP_disabled:
        i2c->write_mode(3);
        break;
    case CP_sink:
        i2c->write_mode(1);
        break;
    case CP_source:
        i2c->write_mode(2);
        break;
    case COMP_FASE_TEST:
        i2c->write_mode(4);
        break;
    case INICIAR:
        i2c->write_mode(0);
        break;
    default:
        break;
    }
 
}
bool Comandos_procesa(char c)
{
    static CMD cmd[1];
    static Estado estado   = INICIO;
    static Palabra palabra = {.max = N_COMANDOS - 1, .min = 0, .n = 0};
    static Numero numero[9] = {0}; // Arreglo de numeros para los parametros
    static int parametroRecibido = 0;
    bool encontrado                  = 0;
    // uart->write('\n');
    if (esTerminador(c)) {
        /// Comprueba si detectó un parametro valido///
        encontrado = 1;
        if (DESCO != getCMD(&palabra)) {
            cmd->code = (cmd_c_parametro(&palabra) > parametroRecibido) ? FaltanParametros : SobranParametros;
            if ((cmd_c_parametro(&palabra) == parametroRecibido)) {
                cmd->code = CodigoValido;
            }
        } else {
            cmd->code = SyntaxError;
        }
        /// Actualiza cmd///
        cmd->cmd = getCMD(&palabra);
        for (int i = 0; i <= parametroRecibido; i++)
            cmd->parametro[i] = getNumero(&numero[i]);
        /// Cond. Iniciales///
        estado            = buscaCMD;
        parametroRecibido = 0;
        palabraCLR(&palabra);
        procesar_cmd(cmd);
    } else {
        switch (estado) {
        case ERROR:
            break;
        case INICIO: // Para que los espacios en blanco no afecten, y diferenciar de esperar un parametro
            if (isalpha(c)) {
                estado = buscaCMD;
                agregarLetra(&palabra, c);
            } else if (isblank(c)) {
                estado = buscaCMD;
            } else {
                estado = ERROR;
            }
            break;
        case buscaCMD:
            if (c == ' ') {
                estado = blank;
            } else if (isalnum(c) || c == '?') {
                agregarLetra(&palabra, c);
            } else {
                estado = ERROR;
            }
            break;
        case buscaNUM:
            if (isblank(c)) {
                estado = blank;
            } else if (!agregarDig(&numero[parametroRecibido-1], c)) {
                estado = ERROR;
            }
            break;
        case blank:
            if (agregarDig(&numero[parametroRecibido], c)) {
                estado = buscaNUM;
                parametroRecibido++;
            } else if (!isblank(c)) {
                estado = ERROR;
            }
            break;
        }
    }
    return encontrado;
}
