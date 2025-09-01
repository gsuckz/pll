#include "interfaz.h"
#include <Arduino.h>

// ================== Definiciones ==================
#define QUEUE_SIZE 16 // tamaño máximo de la cola

// Códigos de teclas
#define NOTHING 0
#define SELECT_KEY 1
#define LEFT_KEY 2
#define DOWN_KEY 3
#define UP_KEY 4
#define RIGHT_KEY 5

// ================== Cola de eventos ==================
typedef enum { EVENT_NONE, EVENT_SELECT, EVENT_LEFT, EVENT_DOWN, EVENT_UP, EVENT_RIGHT } KEY_EVENT;

KEY_EVENT eventQueue[QUEUE_SIZE];
volatile int queueHead = 0;
volatile int queueTail = 0;

// ================== Variables ==================
int sensorPin = A0;
int lastKey   = 0;
int button    = 0;

codigoTecla cuantizar(int value)
{
    if (value > 120) return NOTHING;
    if (value > 110) return SELECT_KEY;
    if (value > 95) return LEFT_KEY;
    if (value > 70) return DOWN_KEY;
    if (value > 40) return UP_KEY;
    return RIGHT_KEY;
}

typedef enum {
    IDLE,
    PRESIONADO

} estadoTeclado;
