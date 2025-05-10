#ifndef SINTETIZADOR_H
#define SINTETIZADOR_H

#include <WString.h>

/* Definiciones de tipos públicos */

typedef enum TipoTest_e { CP_snk, CP_src, CP_dis, tipoTest_MAX } TipoTest;

/* Variables públicas*/

extern const String nombresTests[tipoTest_MAX];
extern boolean modoTest; // Leer para entender...
extern TipoTest testActual;

/* Declaración de funciones públicas*/

/**
 * @brief Inicializa el sintetizador
 *
 */
void SintetizadorInicializa(void);

void SintetizadorCambiaFrecuencia(int freq);
void SintetizadorCambiaModo(int mode);
int SintetizadorLeeModo();

void enviari2c(uint8_t valor)

#endif /*SINTETIZADOR_H*/
