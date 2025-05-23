#ifndef BUFFER_H
#define BUFFER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#define BUFFER_MAX 64

/**
 * @brief Buffer de caracteres
 *
 */
typedef struct Buffer Buffer;

/**
 * @brief Estado de buffer
 *
 */
struct Buffer {
    char chars[BUFFER_MAX]; //!< Memoria
    uint8_t escritura;      //!< Índice de escritura
    uint8_t lectura;        //!< Índice de lectura
};

/**
 * @brief Escribe un caracter en el Buffer, si esta lleno devuleve false sin
 * efecto
 *
 *
 * @param self
 * @param caracter
 * @return true Escribio el Buffer
 * @return false Sin efecto (Buffer lleno)
 */
bool buffer_escribir(Buffer *self, char caracter);
/**
 * @brief Guarda en la direccion caracter_p el siguiente caracter a leer del
 * Buffer en el caso de que este vacio devuelve False
 *
 * @param self
 * @param caracter_p
 * @return true Se guardo el caracter leido en caracter_p
 * @return false No habia para leer
 */
bool buffer_leer(Buffer *self, char *caracter_p);
/**
 * @brief Limpia el buffer. Luego de llamar a esta funcion, el buffer no tiene
 * nada para leer
 *
 * @param self
 */
void buffer_clr(Buffer *self);

#ifdef __cplusplus
}
#endif

#endif
