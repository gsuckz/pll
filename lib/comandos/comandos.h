
#ifndef COMANDOS_H
#define COMANDOS_H


#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
typedef struct UART{
    void (*write_string)(const char *str);
    void (*write_numero)(int num);
    void (*write)(int c); 
}UART;

void Comandos_init(const UART *uart);

/**
 * @brief Envia un caracter para ser procesado
 * 
 * @param cmd 
 * @param c 
 * @return true 
 * @return false 
 */
bool Comandos_procesa(char c);


#ifdef __cplusplus
}
#endif

#endif