#ifndef BLUEPILLHAL_H
#define BLUEPILLHAL_H
#include <stdbool.h>
#include <stdint.h>

typedef enum BP_PuertoPin
{
    A0,
    A1,
    A2,
    A3,
    A4,
    A5,
    A6,
    A9,
    A10,
    B5,
    B6,
    B7,
    B8,
    B9,
    B10,
    B12,
    C13,
    PINLIMITE
} BP_PuertoPin;

typedef enum BP_ModoPin
{
    IN_PULL_UP,
    OUT_2MHz,
    IN,
    AO_PP
} BP_ModoPin;

void BP_inicio();
bool BP_Pin_read(BP_PuertoPin pin);

/**
 * @brief Activa el puerto y pone el pin en el modo indicado
 *
 * @param pin
 * @param modo
 */
void BP_Pin_mode(BP_PuertoPin pin, BP_ModoPin modo);
void BP_Pin_set(BP_PuertoPin pin, bool valor);
/**
 * @brief Retarda un tiempo en milisegundos, bloquea.
 *
 * @param tiempo Tiempo en milisegundos
 */
void BP_delay(uint32_t tiempo);

unsigned getTicks(void);
void set_tick_handler( void (*funcion) (void));

#endif