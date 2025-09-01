#ifndef TECLADO_H
#define TECLADO_H

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#ifdef __cplusplus
extern "C" {
#endif

#define KEV_SELECT_DOWN (1 << 0)
#define KEV_SELECT_UP (1 << 1)
#define KEV_LEFT_DOWN (1 << 2)
#define KEV_LEFT_UP (1 << 3)
#define KEV_DOWN_DOWN (1 << 4)
#define KEV_DOWN_UP (1 << 5)
#define KEV_UP_DOWN (1 << 6)
#define KEV_UP_UP (1 << 7)
#define KEV_RIGHT_DOWN (1 << 8)
#define KEV_RIGHT_UP (1 << 9)

#define KEV_ALL_DOWN (KEV_SELECT_DOWN | KEV_LEFT_DOWN | KEV_DOWN_DOWN | KEV_UP_DOWN | KEV_RIGHT_DOWN)
#define KEV_ALL_UP (KEV_SELECT_UP | KEV_LEFT_UP | KEV_DOWN_UP | KEV_UP_UP | KEV_RIGHT_UP)

typedef struct TecladoTaskParams {
    EventGroupHandle_t eventGroup; // Grupo de eventos para comunicar los eventos de teclado
    int pollIntervalMs;            // Intervalo de sondeo en milisegundos
    int pin;                       // Pin analógico donde está conectado el teclado
} TecladoTaskParams;

/// @brief Tarea de teclado
/// @param TecladoTaskParams * Puntero a la estructura de parámetros de la tarea
void tecladoTask(void *tecladoTaskParams);

#define TECLADO_TASK_STACK_SIZE (configMINIMAL_STACK_SIZE + 100)

#ifdef __cplusplus
}
#endif

#endif /*TECLADO_H*/