#include "test_numeros.h"

int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_numero_solo_recibe_numeros);
    RUN_TEST(test_numero_1);
    RUN_TEST(test_numero_12);
    RUN_TEST(test_numero_22);
    RUN_TEST(test_numero_0);
    RUN_TEST(test_numero_235453);
    RUN_TEST(test_toString_123123);
    RUN_TEST(test_toString_1231234);
    RUN_TEST(test_toString_0);
    UNITY_END();
    return 0;
}