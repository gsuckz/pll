#include "comandos.h"
#include <ctype.h>
#include <stdint.h>
#include "buffer.h"
#include "numeros.h"


#define N_COMANDOS 7
#define MAX_N_PARAMETROS 1

typedef enum Command{
    APAGAR, 
    BARRER,
    FREC,
    FRECUENCIA,
    FRECUENCIAq,
    FRECq, //ANG?
    IDq, //ID?
    RESET_CMD,
    DESCO=255
}Command;

typedef enum Codigo{
    OK,
    none,
    SyntaxError,
    FaltanParametros,
    SobranParametros
}Codigo;

typedef struct CMD{
    Command cmd;
    int parametro[9];
    Codigo code;
}CMD;



static char const * const tabla_cmd[N_COMANDOS] = {
    "apagar",
    "barrer",
    "frec\n1",
    "frecuencia\n1",
    "frecuencia?",
    "frec?",
    "id?",
    "reset"
};


typedef enum Estado {
    INICIO,
    buscaCMD,
    buscaNUM,
    blank,
    ERROR
}Estado;

typedef struct  Palabra
{
   int min;
   int max;
   int n;
}Palabra;

static const UART *uart;
static const I2C *i2c;

void Comandos_init(const UART *uart_)
{
    uart = uart_;
}
void comandos_i2c(const I2C *i2c_){
    i2c = i2c_;
}

static void agregarLetra(Palabra * palabra, char c){   
    if (palabra->max < palabra->min) {
        return; //Si no hubo ninguna coincidencia la ultima vez, no hace nada.
    }
    if (isalpha(c)){
        c = tolower(c);
    }
    for (int i = palabra->min; i<= palabra->max; i++){ //  Busco la primera coincidencia.
        char caracter_tabla =  tabla_cmd[i][palabra->n];
        if (c == caracter_tabla){
            palabra->min = i;
            break;
        }
    }

    for (int i = palabra->min; i<=  palabra->max; i++){// Busco la primera diferencia.
        char caracter_tabla =  tabla_cmd[i][palabra->n];
        if (c != caracter_tabla) {
            palabra->max = i - 1;
            break;
        }
    }
    palabra->n++; 
}
static void palabraCLR(Palabra * palabra){
    palabra->max = N_COMANDOS - 1;
    palabra->min = 0;
    palabra->n = 0;
}
static Command getCMD(Palabra * palabra){
    Command const comando = ((palabra->max >= palabra->min) && ((tabla_cmd[palabra->min][palabra->n] == '\0' ) 
                                                            ||  (tabla_cmd[palabra->min][palabra->n] == '\n' )))

     ? palabra->min : DESCO;
    return comando;
}
static uint8_t cmd_c_parametro(Palabra * palabra){
    uint8_t cantidadParametros = 0;
    if (tabla_cmd[palabra->min][palabra->n ] == '\n'){
        cantidadParametros = (tabla_cmd[palabra->min][palabra->n + 1] - '0'); 
    }else{
        cantidadParametros = 0;
    }
    return cantidadParametros;
}
static bool esTerminador(char c){
    return (c == '\n' ); //|| c == '\r' (El primero que se envía es '\n' por lo que no se consulta el otro)
        //no molesta despues??
}

static void procesar_cmd(CMD * cmd){  
    switch (cmd->cmd)
    {        case FREC:
             case FRECUENCIA: //FALLTHRU
            if(cmd->parametro[0] >= 11800 || cmd->parametro[0] <= 106000 ){ //&& (cmd->code = OK)
                    //set_servo_angle(cmd->parametro[0]); Escribir en I2C valor del divisor de frecuencia
                    i2c->write_freq(cmd->parametro[0]);
                    uart->write_string("Frecuencia fijada en: ");
                    uart->write_numero(cmd->parametro[0]);
                    uart->write('\n'); 
                    uart->write('\r'); 
                }else{
                uart->write_string("Frecuencia, ingrege un valor entero entre 10600-11800 MHz\n\r"); 
                }                     
        break;case FRECq:
            case FRECUENCIAq: //FALLTHRU
            uart->write_string("Frecuencia fijada en: ");
            uart->write_numero( i2c->read_freq() );//get_servo_angle()); leer registro del Zarlink SP5769
            uart->write('\n');           
            uart->write('\r');           
        break;case IDq:
            uart->write_string("Sintetizador de Frecuencias en Banda Ku v0.1\n\r");
        break;default:
        break;
    }
    switch (cmd->code){
        case SyntaxError:
            uart->write_string("Error de Syntaxis\r\n");
        break;case FaltanParametros:
            uart->write_string("Se necesitan mas parámetros\n\r");
        break; case SobranParametros:
            uart->write_string("Demasiados Parametros para el comando\n\r");
        break; default:
        break;
    }
}
bool Comandos_procesa(char c){
    static CMD cmd[1];
    static Estado estado = INICIO;
    static Palabra  palabra = {.max = N_COMANDOS - 1 , .min = 0, .n = 0 };
    static Numero numero;
    static uint8_t parametroRecibido = 0 ;
    bool encontrado = 0;
    //uart->write('\n');
    if (esTerminador(c)){
        ///Comprueba si detectó un parametro valido///
        encontrado = 1;
        if(DESCO != getCMD(&palabra)){
            cmd->code = (cmd_c_parametro(&palabra) > parametroRecibido) ? FaltanParametros : SobranParametros; 
            if((cmd_c_parametro(&palabra) == parametroRecibido)) {
                cmd->code = OK;
            }
        }else{
            cmd->code = SyntaxError;
        }
        ///Actualiza cmd///
        cmd->cmd = getCMD(&palabra);
        for (int i = 0;i<= parametroRecibido; i++) cmd->parametro[i] = getNumero(&numero);
        ///Cond. Iniciales///
        estado = buscaCMD;
        parametroRecibido = 0;
        palabraCLR(&palabra);
        procesar_cmd(cmd);
    }else{
        switch (estado) {
        case ERROR:
        break;case INICIO: //Para que los espacios en blanco no afecten, y diferenciar de esperar un parametro
            if (isalpha(c)){
                estado = buscaCMD;
                agregarLetra(&palabra,c);
            }else if (isblank(c)){
                estado = buscaCMD;
            }else{
                estado = ERROR;
            }
        break;case buscaCMD:
            if (c == ' '){
                estado = blank;
            }else if(isalnum(c) || c== '?') {
                agregarLetra(&palabra,c);          
            }else{
            estado = ERROR;
            }
        break;case buscaNUM:
            if (isblank(c)){
                estado = blank;
            }else if(!agregarDig(&numero,c)) {
                estado = ERROR;
            }
        break;case blank: 
            if (agregarDig(&numero,c)){
                estado = buscaNUM;
                parametroRecibido++;
            }else if (!isblank(c)){
                estado = ERROR;
            }
        break;
        }
    }
    return encontrado;
}











