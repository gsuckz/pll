#include <BluePillHal.h>
#include <stm32f1xx.h>

static void (*funcion_ticks)(void) = 0;

void SysTick_Handler(void);

void BP_inicio() {
  SystemCoreClockUpdate();
  uint32_t const CUENTAS_POR_MILISEG = SystemCoreClock / 1000;
  SysTick_Config(CUENTAS_POR_MILISEG);
}

typedef struct Pin {
  GPIO_TypeDef *puerto;
  int npin;
} Pin;

static Pin const pines[PINLIMITE] = {[A0] = {.puerto = GPIOA, .npin = 0},
                                     [A1] = {.puerto = GPIOA, .npin = 1},
                                     [A2] = {.puerto = GPIOA, .npin = 2},
                                     [A3] = {.puerto = GPIOA, .npin = 3},
                                     [A4] = {.puerto = GPIOA, .npin = 4},
                                     [A5] = {.puerto = GPIOA, .npin = 5},
                                     [A6] = {.puerto = GPIOA, .npin = 6},
                                     [A9] = {.puerto = GPIOA, .npin = 9},
                                     [A10] = {.puerto = GPIOA, .npin = 10},
                                     [B12] = {.puerto = GPIOB, .npin = 12},
                                     [C13] = {.puerto = GPIOC, .npin = 13},
                                     [B5] = {.puerto = GPIOB, .npin = 5},
                                     [B6] = {.puerto = GPIOB, .npin = 6},
                                     [B7] = {.puerto = GPIOB, .npin = 7},
                                     [B8] = {.puerto = GPIOB, .npin = 8},
                                     [B9] = {.puerto = GPIOB, .npin = 9},
                                     [B10] = {.puerto = GPIOB, .npin = 10}

};

bool BP_Pin_read(BP_PuertoPin pin) {
  return pines[pin].puerto->IDR & (0x1UL << pines[pin].npin);
}

void BP_Pin_mode(BP_PuertoPin pin, BP_ModoPin modo) {
  uint32_t bitsModo;
  int const offset_habilitacion = (((uint32_t)(pines[pin].puerto) >> 10) & 0xF);
  uint32_t const mascara_habilitacion = 1UL << offset_habilitacion;
  RCC->APB2ENR |= mascara_habilitacion;
  switch (modo) {
  case IN_PULL_UP:
    bitsModo = 0b1000;
    pines[pin].puerto->ODR |= (0x1UL << pines[pin].npin);
    break;
  case OUT_2MHz:
    bitsModo = 0b0010;
    break;
  case IN:
    bitsModo = 0b0100;
    break;
  case AO_PP:
    bitsModo = 0b1010;
    break;
  default:
    bitsModo = 0;
    break;
  }

  int const OFFSET_CONFIG = ((pines[pin].npin % 8) * 4);
  int const MASCARA_CONFIG = 0b1111;
  uint32_t volatile *const CR =
      (pines[pin].npin < 8) ? &pines[pin].puerto->CRL : &pines[pin].puerto->CRH;

  *CR =
      (*CR & ~(MASCARA_CONFIG << OFFSET_CONFIG)) | (bitsModo << OFFSET_CONFIG);
}
void BP_Pin_set(BP_PuertoPin pin, bool valor) {
  if (valor)
    pines[pin].puerto->BSRR = (0x1UL << pines[pin].npin);
  else
    pines[pin].puerto->BRR = (0x1UL << pines[pin].npin);
}

static uint32_t volatile ticks;
void BP_delay(uint32_t tiempo) {
  uint32_t const ticks_inicial = ticks;
  uint32_t tiempo_transcurrido = ticks - ticks_inicial;
  while (tiempo_transcurrido < tiempo) {
    __WFI();
    tiempo_transcurrido = ticks - ticks_inicial;
  }
}

void SysTick_Handler(void) {
  ++ticks;
  if (funcion_ticks) {
    funcion_ticks();
  }
}

void set_tick_handler(void (*funcion)(void)) { funcion_ticks = funcion; }

unsigned getTicks(void) { return ticks; }
