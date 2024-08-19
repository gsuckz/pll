#include <UART.h>
#include <TIMER.h>
#include "BluePillHal.h"

int main (void){
    UART_init();
    TIM3_init();
    BP_Pin_mode(C13,OUT_2MHz);

    
    for(;;);

    return 0;
}

