#include <Arduino.h>

void setup(void)
{
    pinMode(2,OUTPUT);
}
void loop(void)
{
    static bool a=false;
    static unsigned long t0=0;

    const unsigned long t = millis();

    if(t-t0 >= 500UL){
        digitalWrite(2,a);
        a = !a;
        t0=t;
    }
}