
#ifndef COMANDOS_H
#define COMANDOS_H
#define N_COMANDOS 7
#define MAX_N_PARAMETROS 1
#include <stdbool.h>

typedef enum Command{
    ANG,
    ANGULO,
    ANGULOq,
    ANGq, //ANG?
    APAGAR, 
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

/**
 * @brief Envia un caracter para ser procesado
 * 
 * @param cmd 
 * @param c 
 * @return true 
 * @return false 
 */
bool getCommand(CMD * cmd, char c);

#endif