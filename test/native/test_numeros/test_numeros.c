#include <unity.h>
#include "numeros.h"


Numero numero;
void setUp(){
    getNumero(&numero);
}
void tearDown(){

}

void test_numero_solo_recibe_numeros (void){
    TEST_ASSERT_FALSE(agregarDig(&numero,' '));
    TEST_ASSERT_FALSE(agregarDig(&numero,'\0'));
    TEST_ASSERT_FALSE(agregarDig(&numero,'/'));
    TEST_ASSERT_FALSE(agregarDig(&numero,'?'));
    TEST_ASSERT_FALSE(agregarDig(&numero,'\n'));
    TEST_ASSERT_FALSE(agregarDig(&numero,'\r'));
    TEST_ASSERT_FALSE(agregarDig(&numero,'A'));
    TEST_ASSERT_FALSE(agregarDig(&numero,'a'));

}
void test_numero_1 (void){
    TEST_ASSERT_TRUE(agregarDig(&numero,'1'));
    TEST_ASSERT_EQUAL(1,getNumero(&numero));
}

void test_numero_12 (void){
    TEST_ASSERT_TRUE(agregarDig(&numero,'1'));
    TEST_ASSERT_TRUE(agregarDig(&numero,'2'));
    TEST_ASSERT_EQUAL(12,getNumero(&numero));
}
void test_numero_22 (void){
    TEST_ASSERT_TRUE(agregarDig(&numero,'2'));
    TEST_ASSERT_TRUE(agregarDig(&numero,'2'));
    TEST_ASSERT_EQUAL(22,getNumero(&numero));
}

void test_numero_0 (void){
    TEST_ASSERT_TRUE(agregarDig(&numero,'0'));
    TEST_ASSERT_TRUE(agregarDig(&numero,'0'));
    TEST_ASSERT_EQUAL(0,getNumero(&numero));
}
void test_numero_235453 (void){
    TEST_ASSERT_TRUE(agregarDig(&numero,'2'));
    TEST_ASSERT_TRUE(agregarDig(&numero,'3'));
    TEST_ASSERT_TRUE(agregarDig(&numero,'5'));
    TEST_ASSERT_TRUE(agregarDig(&numero,'4'));
    TEST_ASSERT_TRUE(agregarDig(&numero,'5'));
    TEST_ASSERT_TRUE(agregarDig(&numero,'3'));
    TEST_ASSERT_EQUAL(235453,getNumero(&numero));
}
void test_toString_123123 (void){
    char numeroString[10];
    toString(123123,numeroString,10);
    TEST_ASSERT_EQUAL_STRING("123123",numeroString);
}
void test_toString_1231234 (void){
    char numeroString[10];
    toString(1231234,numeroString,10);
    TEST_ASSERT_EQUAL_STRING("1231234",numeroString);
}
void test_toString_0 (void){
    char numeroString[10];
    toString(0,numeroString,10);
    TEST_ASSERT_EQUAL_STRING("0",numeroString);
}

