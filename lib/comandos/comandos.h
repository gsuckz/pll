
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

typedef struct I2C{
    void (*write_freq)(int valor);
    int (*read_state)();
}I2C;

void comandos_i2c(const I2C *i2c);





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