#include "src/numeros.h"
#include "C:/Ruby27-x64/lib/ruby/gems/2.7.0/gems/ceedling-0.31.1/vendor/unity/src/unity.h"




Numero numero;

void setUp(){

    getNumero(&numero);

}

void tearDown(){



}



void test_numero_solo_recibe_numeros (void){

    do {if (!(agregarDig(&numero,' '))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(14)));}} while(0);

    do {if (!(agregarDig(&numero,'\0'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(15)));}} while(0);

    do {if (!(agregarDig(&numero,'/'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(16)));}} while(0);

    do {if (!(agregarDig(&numero,'?'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(17)));}} while(0);

    do {if (!(agregarDig(&numero,'\n'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(18)));}} while(0);

    do {if (!(agregarDig(&numero,'\r'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(19)));}} while(0);

    do {if (!(agregarDig(&numero,'A'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(20)));}} while(0);

    do {if (!(agregarDig(&numero,'a'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(21)));}} while(0);



}

void test_numero_1 (void){

    do {if ((agregarDig(&numero,'1'))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(25)));}} while(0);

    UnityAssertEqualNumber((UNITY_INT)((1)), (UNITY_INT)((getNumero(&numero))), (

   ((void *)0)

   ), (UNITY_UINT)(26), UNITY_DISPLAY_STYLE_INT);

}



void test_numero_12 (void){

    do {if ((agregarDig(&numero,'1'))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(30)));}} while(0);

    do {if ((agregarDig(&numero,'2'))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(31)));}} while(0);

    UnityAssertEqualNumber((UNITY_INT)((12)), (UNITY_INT)((getNumero(&numero))), (

   ((void *)0)

   ), (UNITY_UINT)(32), UNITY_DISPLAY_STYLE_INT);

}

void test_numero_22 (void){

    do {if ((agregarDig(&numero,'2'))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(35)));}} while(0);

    do {if ((agregarDig(&numero,'2'))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(36)));}} while(0);

    UnityAssertEqualNumber((UNITY_INT)((22)), (UNITY_INT)((getNumero(&numero))), (

   ((void *)0)

   ), (UNITY_UINT)(37), UNITY_DISPLAY_STYLE_INT);

}



void test_numero_0 (void){

    do {if ((agregarDig(&numero,'0'))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(41)));}} while(0);

    do {if ((agregarDig(&numero,'0'))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(42)));}} while(0);

    UnityAssertEqualNumber((UNITY_INT)((0)), (UNITY_INT)((getNumero(&numero))), (

   ((void *)0)

   ), (UNITY_UINT)(43), UNITY_DISPLAY_STYLE_INT);

}

void test_numero_235453 (void){

    do {if ((agregarDig(&numero,'2'))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(46)));}} while(0);

    do {if ((agregarDig(&numero,'3'))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(47)));}} while(0);

    do {if ((agregarDig(&numero,'5'))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(48)));}} while(0);

    do {if ((agregarDig(&numero,'4'))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(49)));}} while(0);

    do {if ((agregarDig(&numero,'5'))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(50)));}} while(0);

    do {if ((agregarDig(&numero,'3'))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(51)));}} while(0);

    UnityAssertEqualNumber((UNITY_INT)((235453)), (UNITY_INT)((getNumero(&numero))), (

   ((void *)0)

   ), (UNITY_UINT)(52), UNITY_DISPLAY_STYLE_INT);

}

void test_toString_123123 (void){

    char numeroString[10];

    toString(123123,numeroString,10);

    UnityAssertEqualString((const char*)(("123123")), (const char*)((numeroString)), (

   ((void *)0)

   ), (UNITY_UINT)(57));

}

void test_toString_1231234 (void){

    char numeroString[10];

    toString(1231234,numeroString,10);

    UnityAssertEqualString((const char*)(("1231234")), (const char*)((numeroString)), (

   ((void *)0)

   ), (UNITY_UINT)(62));

}

void test_toString_0 (void){

    char numeroString[10];

    toString(0,numeroString,10);

    UnityAssertEqualString((const char*)(("0")), (const char*)((numeroString)), (

   ((void *)0)

   ), (UNITY_UINT)(67));

}
