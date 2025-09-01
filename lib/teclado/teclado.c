#include "teclado.h"
#include <Arduino.h>

typedef enum CodigoTecla {
    NOTHING    = 0,
    SELECT_KEY = 1,
    LEFT_KEY   = 2,
    DOWN_KEY   = 3,
    UP_KEY     = 4,
    RIGHT_KEY  = 5
} CodigoTecla;

typedef enum EstadoTeclado { IDLE, PRESSED } EstadoTeclado;

void tecladoTask(void *tecladoTaskParams)
{
    TecladoTaskParams *params     = tecladoTaskParams;
    EventGroupHandle_t eventGroup = params->eventGroup;
    int pollIntervalMs            = params->pollIntervalMs;
    int pin                       = params->pin;
    EstadoTeclado estado          = IDLE;
    CodigoTecla lastKey           = NOTHING;
    CodigoTecla currentKey        = NOTHING;
    CodigoTecla buffer[4]         = {NOTHING, NOTHING, NOTHING, NOTHING};
    int index                     = 0;
    for (;;) {
        int lectura = analogRead(pin);
        if (lectura > 1215) {
            currentKey = NOTHING;
        } else if (lectura > 1120) {
            currentKey = SELECT_KEY;
        } else if (lectura > 975) {
            currentKey = LEFT_KEY;
        } else if (lectura > 825) {
            currentKey = DOWN_KEY;
        } else if (lectura > 375) {
            currentKey = UP_KEY;
        } else {
            currentKey = RIGHT_KEY;
        }

        buffer[index] = currentKey;
        index         = (index + 1) % 4;

        if (buffer[0] == buffer[1] && buffer[0] == buffer[2] && buffer[0] == buffer[3]) {
            switch (estado) {
            case IDLE:
                if (currentKey != NOTHING) {
                    digitalWrite(12, HIGH);
                    lastKey = currentKey;
                    estado  = PRESSED;
                    switch (currentKey) {
                    case SELECT_KEY:
                        xEventGroupSetBits(eventGroup, KEV_SELECT_DOWN);
                        break;
                    case LEFT_KEY:
                        xEventGroupSetBits(eventGroup, KEV_LEFT_DOWN);
                        break;
                    case DOWN_KEY:
                        xEventGroupSetBits(eventGroup, KEV_DOWN_DOWN);
                        break;
                    case UP_KEY:
                        xEventGroupSetBits(eventGroup, KEV_UP_DOWN);
                        break;
                    case RIGHT_KEY:
                        xEventGroupSetBits(eventGroup, KEV_RIGHT_DOWN);
                        break;
                    default:
                        break;
                    }
                }
                break;
            case PRESSED:
                if (currentKey == NOTHING) {
                    digitalWrite(12, LOW);
                    estado = IDLE;
                    switch (lastKey) {
                    case SELECT_KEY:
                        xEventGroupSetBits(eventGroup, KEV_SELECT_UP);
                        break;
                    case LEFT_KEY:
                        xEventGroupSetBits(eventGroup, KEV_LEFT_UP);
                        break;
                    case DOWN_KEY:
                        xEventGroupSetBits(eventGroup, KEV_DOWN_UP);
                        break;
                    case UP_KEY:
                        xEventGroupSetBits(eventGroup, KEV_UP_UP);
                        break;
                    case RIGHT_KEY:
                        xEventGroupSetBits(eventGroup, KEV_RIGHT_UP);
                        break;
                    default:
                        break;
                    }
                    lastKey = NOTHING;
                }
                break;
            default:
                estado  = IDLE;
                lastKey = NOTHING;
                break;
            }
        }
        vTaskDelay(pdMS_TO_TICKS(pollIntervalMs));
    }
}