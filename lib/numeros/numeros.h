#ifndef NUMEROS_H
#define NUMEROS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

typedef struct Numero {
    uint8_t digitos[10];
    uint8_t index;
} Numero;

/**
 * @brief Agrega un digito al numero
 *
 * @param numero
 * @param c
 * @return true Si pudo agregarlo
 * @return false Si no pudo, esto puede ser porque esta lleno el arreglo o porque era invàlido
 */
bool agregarDig(Numero *numero, char c);
/**
 * @brief Devuleve el numero guardado y resetea para guardar un nuevo numero
 *
 * @param numero
 * @return int
 */
int getNumero(Numero *numero);
/**
 * @brief Guarda el numero como un String
 *
 * @param numero
 * @param char_n
 * @param size
 * @return uint8_t
 */
uint8_t toString(int numero, char *char_n, uint8_t size);

#ifdef __cplusplus
}
#endif

#endif