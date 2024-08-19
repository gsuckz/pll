#ifndef TIMER_H
#define TIMER_H
#include <stdint.h>

/**
 * @brief Inicializa el Timer 3 para la generación de PWM.
 */
void    TIM3_init(void);

/**
 * @brief Establece el ángulo del servo motor.
 * @param angle El ángulo deseado para el servo motor (0-180).
 */
void set_servo_angle(uint32_t angle); //Comprobar que valor este entre 180!

/**
 * @brief Obtiene el ángulo actual del servomotor.
 * @return El ángulo actual del servomotor.
 */
int get_servo_angle(void);
#endif