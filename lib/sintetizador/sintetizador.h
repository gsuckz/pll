#ifndef SINTETIZADOR_H
#define SINTETIZADOR_H
// #include <WString.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* Definiciones de tipos públicos */
typedef enum i2cError { ENVIADO = 0, muyLargo, nackAddres, nackData, ERROR, timeout } i2cError;
typedef enum modo { NORMAL, CP_SINK, CP_SOURCE, CP_DISABLE, COMPARADOR_FASE_TEST} modo;
typedef enum TipoTest_e { CP_snk, CP_src, CP_dis, tipoTest_MAX } TipoTest;

/* Variables públicas*/
extern const char *nombresTests[tipoTest_MAX];

extern bool modoTest; // Leer para entender...
extern TipoTest testActual;

/* Declaración de funciones públicas*/

/**
 * @brief Inicializa el sintetizador
 *
 */
void SintetizadorInicializa(void);

void SintetizadorCambiaFrecuencia(int freq);

void SintetizadorCambiaModo(int mode);

int SintetizadorLeeEstado();

int SintetizadorTick(void);

void enviari2c(uint8_t valor); ///????????????

void configurarBarrido(int min, int max, int duracion);

void paraBarrido(void);

#endif /*SINTETIZADOR_H*/
