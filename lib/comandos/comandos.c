#include "comandos.h"
#include <ctype.h>
#include <stdint.h>
#include "buffer.h"
#include "numeros.h"
#include "TIMER.h"

#ifndef PIO_UNIT_TESTING
#include "UART.h"
#endif

static char const * const tabla_cmd[N_COMANDOS] = {
    "ang\n1",
    "angulo\n1",
    "angulo?",
    "ang?",
    "apagar",
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
#ifndef PIO_UNIT_TESTING
    switch (cmd->cmd)
    {        case ANG:
             case ANGULO: //FALLTHRU
            if(cmd->parametro[0] <=180  ){ //&& (cmd->code = OK)
                    set_servo_angle(cmd->parametro[0]);
                    UART_write_string("Angulo fijado en: ");
                    UART_write_numero(cmd->parametro[0]);
                    UART_write('\n'); 
                    UART_write('\r'); 
                }else{
                UART_write_string("Angulo Invalido, ingrege un valor entre 0-180\n\r"); 
                }                     
        break;case ANGq:
            case ANGULOq: //FALLTHRU
            UART_write_string("Angulo fijado en: ");
            UART_write_numero(get_servo_angle()); 
            UART_write('\n');           
            UART_write('\r');           
        break;case IDq:
            UART_write_string("Controlador Servomotor v0.1\n\r");
        break;default:
        break;
    }
    switch (cmd->code){
        case SyntaxError:
            UART_write_string("Error de Syntaxis\r\n");
        break;case FaltanParametros:
            UART_write_string("Se necesitan mas parámetros\n\r");
        break; case SobranParametros:
            UART_write_string("Demasiados Parametros para el comando\n\r");
        break; default:
        break;
    }
#else
    (void) cmd;
#endif
}
bool getCommand(CMD * cmd, char c){
    static Estado estado = INICIO;
    static Palabra  palabra = {.max = N_COMANDOS - 1 , .min = 0, .n = 0 };
    static Numero numero;
    static uint8_t parametroRecibido = 0 ;
    bool encontrado = 0;
    //UART_write('\n');
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











