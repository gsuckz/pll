#include "buffer.h"

/* Declaración de funciones privadas */

/**
 * @brief Calcula la siguiente posición en un buffer circular
 *
 * @param valor
 * @param modulo
 * @return uint8_t valor siguiente modulo `modulo`
 */
static uint8_t siguiente(uint8_t valor, uint8_t modulo);

/**
 * @brief Determina si el buffer está lleno
 *
 * @param self
 * @retval true Buffer lleno
 * @retval false Hay espacio en buffer
 */
static bool BufferLleno(Buffer *self);

/**
 * @brief Determina si el buffer está vacío
 *
 * @param self
 * @retval true Buffer vacío
 * @retval false Hay al menos un elemento disponible en buffer
 */
static bool BufferVacio(Buffer *self);

/* implementación de funciones privadas */

static uint8_t siguiente(uint8_t valor, uint8_t modulo) {
    // quepasa si llega aca la int??0.
    if (!modulo)
        return 0;
    if (valor >= (modulo - 1)) {
        valor = 0;
    } else {
        valor = valor + 1;
    }
    return valor;
}

static bool BufferLleno(Buffer *self) {
    // return (self->escritura + 1) == self->lectura;
    return siguiente(self->escritura, BUFFER_MAX) == self->lectura;  //El siguiente dato escribiria el que se debe leer
}

static bool BufferVacio(Buffer *self) {
    return self->lectura == self->escritura; //Se lee donde se esta por escribir, Buffer lleno impìde que haya un dato nuevo en esa posicion
}

bool buffer_escribir(Buffer *self, char caracter) {
    if (BufferLleno(self))
        return 0; // que pasa si llega aca la interrupcion?
    self->chars[self->escritura] = caracter;
    // Asignar nuevo valor
    self->escritura = siguiente(self->escritura, BUFFER_MAX);
    return true;
}

bool buffer_leer(Buffer *self, char *caracter_p) {
    if (BufferVacio(self))
        return 0;

    *caracter_p = self->chars[self->lectura];
    self->lectura = siguiente(self->lectura, BUFFER_MAX);
    return 1;
}

void buffer_clr(Buffer *self) { self->lectura = self->escritura; }
