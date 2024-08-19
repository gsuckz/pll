#ifndef UART_H
#define UART_H

#include <stdlib.h>
#include <stdbool.h>

/**
 * @brief Esta libreria controla el periferico USART 1 en:
 * Modo asincrònico Full Duplex
 * Pines A09 (Tx) y A10(Rx)
 * Baudrate = 9600
 * 
 */


void UART_init(void);

/**
 * @brief Lee el caracter recibido
 * 
 * @return char 
 */
bool UART_read(char * c);
/**
 * @brief Envía un caracter por el puerto
 * 
 * @param caracter 
 */
bool UART_write(char caracter);

void UART_write_string(char * string);
void UART_write_numero(int numero);

#endif
