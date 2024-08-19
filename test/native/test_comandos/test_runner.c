#include "test_comandos.h"



int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_Comando_invalido);
    RUN_TEST(test_IDq);
    RUN_TEST(test_comadoreste);
    RUN_TEST(test_apagar);
    RUN_TEST(test_apagar_cortado);
    RUN_TEST(test_IDq_c_espacio_al_final);
    RUN_TEST(test_IDq_c_espacios_al_final);
    RUN_TEST(test_ID_incompleto);
    RUN_TEST(test_ANG_q);
    RUN_TEST(test_ANG_q_c_espacio);
    RUN_TEST(test_ANG_q_c_numero);
    RUN_TEST(test_ANG_sin_parametro);
    RUN_TEST(test_ANG_Space);
    RUN_TEST(test_ANG_c_numero);
    RUN_TEST(test_ANG_c_numero_c_2_espacios);
    RUN_TEST(test_ANG_c_numero_c_espacios_al_final);
    UNITY_END();
    return 0;
}