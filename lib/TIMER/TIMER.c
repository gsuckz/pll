#include <stm32f1xx.h>
#include "BluePillHal.h"
#include <string.h>

#define PERIODO_PWM 20000  // Período del PWM en microsegundos
#define ANCHO_PULSO_MIN 500  // Ancho de pulso mínimo en microsegundos
#define ANCHO_PULSO_MAX 2500  // Ancho de pulso máximo en microsegundos

void TIM3_init(void) {
  // Habilitar el reloj para TIM3 
  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
  // Configurar el Timer en modo PWM
  TIM3->CR1 = 0;  // Limpiar el registro de control
  TIM3->CR2 = 0;  // Limpiar el registro de control 2
  TIM3->CCMR1 = TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;  // Modo PWM1 en canal 1
  TIM3->CCER = TIM_CCER_CC1E;  // Habilitar la salida del canal 1
  TIM3->PSC = (SystemCoreClock / 1000000) - 1;  // Configurar el preescaler para una frecuencia de 1MHz
  TIM3->ARR = PERIODO_PWM - 1;  // Configurar el período del PWM
  // Configurar el pin de salida del PWM (PA6)
  GPIOA->CRL &= ~(GPIO_CRL_CNF6 | GPIO_CRL_MODE6);  // Limpiar los bits de configuración del pin
  GPIOA->CRL |= GPIO_CRL_CNF6_1 | GPIO_CRL_MODE6_1;  // Modo de salida alternativa push-pull
  // Iniciar el Timer
  TIM3->CR1 |= TIM_CR1_CEN;
}

void set_servo_angle(uint32_t angle) {
  // Calcular el ancho de pulso correspondiente al ángulo
  uint32_t ancho_pulso = ANCHO_PULSO_MIN + ((ANCHO_PULSO_MAX - ANCHO_PULSO_MIN) * angle) / 180;
  // Configurar el valor del registro de comparación del canal 1 para establecer el ancho de pulso
  TIM3->CCR1 = ancho_pulso;
}

int get_servo_angle(void){    // Cálculo del ángulo actual del servomotor
  return ((TIM3->CCR1 - 500) * 180)/2000;
}
