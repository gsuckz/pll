#include "src/numeros.h"
#include "src/comandos.h"
#include "C:/Ruby27-x64/lib/ruby/gems/2.7.0/gems/ceedling-0.31.1/vendor/unity/src/unity.h"


CMD cmd;



void setUp(void){

    do {if ((getCommand(&cmd,'\n'))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(8)));}} while(0);

}

void tearDown(void){

    do {if ((getCommand(&cmd,'\n'))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(11)));}} while(0);

}

void test_Comando_invalido (void){

    do {if (!(getCommand(&cmd,'a'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(14)));}} while(0);

    do {if (!(getCommand(&cmd,'r'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(15)));}} while(0);

    do {if (!(getCommand(&cmd,'c'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(16)));}} while(0);

    do {if (!(getCommand(&cmd,'a'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(17)));}} while(0);

    do {if (!(getCommand(&cmd,' '))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(18)));}} while(0);

    do {if (!(getCommand(&cmd,'a'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(19)));}} while(0);

    do {if (!(getCommand(&cmd,'n'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(20)));}} while(0);

    do {if (!(getCommand(&cmd,'g'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(21)));}} while(0);

    do {if (!(getCommand(&cmd,'?'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(22)));}} while(0);

    do {if ((getCommand(&cmd,'\n'))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(23)));}} while(0);

    UnityAssertEqualNumber((UNITY_INT)((SyntaxError)), (UNITY_INT)((cmd.code)), (

   ((void *)0)

   ), (UNITY_UINT)(24), UNITY_DISPLAY_STYLE_INT);

    do {if (!(getCommand(&cmd,'r'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(25)));}} while(0);

}

void test_IDq (void){

    do {if (!(getCommand(&cmd,'a'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(28)));}} while(0);

    do {if (!(getCommand(&cmd,'b'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(29)));}} while(0);

    do {if ((getCommand(&cmd,'\n'))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(30)));}} while(0);

    do {if (!(getCommand(&cmd,'i'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(31)));}} while(0);

    do {if (!(getCommand(&cmd,'d'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(32)));}} while(0);

    do {if (!(getCommand(&cmd,'?'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(33)));}} while(0);

    do {if ((getCommand(&cmd,'\n'))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(34)));}} while(0);

    UnityAssertEqualNumber((UNITY_INT)((IDq)), (UNITY_INT)((cmd.cmd)), (

   ((void *)0)

   ), (UNITY_UINT)(35), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((cmd.parametro[0])), (UNITY_INT)((0)), (

   ((void *)0)

   ), (UNITY_UINT)(36), UNITY_DISPLAY_STYLE_INT);

}



void test_comadoreste (void){

    do {if (!(getCommand(&cmd,'r'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(40)));}} while(0);

    do {if (!(getCommand(&cmd,'e'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(41)));}} while(0);

    do {if (!(getCommand(&cmd,'s'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(42)));}} while(0);

    do {if (!(getCommand(&cmd,'e'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(43)));}} while(0);

    do {if (!(getCommand(&cmd,'t'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(44)));}} while(0);

    do {if ((getCommand(&cmd,'\n'))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(45)));}} while(0);

    UnityAssertEqualNumber((UNITY_INT)((RESET)), (UNITY_INT)((cmd.cmd)), (

   ((void *)0)

   ), (UNITY_UINT)(46), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((cmd.parametro[0])), (UNITY_INT)((0)), (

   ((void *)0)

   ), (UNITY_UINT)(47), UNITY_DISPLAY_STYLE_INT);

}



void test_apagar (void){

    do {if (!(getCommand(&cmd,'a'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(51)));}} while(0);

    do {if (!(getCommand(&cmd,'p'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(52)));}} while(0);

    do {if (!(getCommand(&cmd,'a'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(53)));}} while(0);

    do {if (!(getCommand(&cmd,'g'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(54)));}} while(0);

    do {if (!(getCommand(&cmd,'a'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(55)));}} while(0);

    do {if (!(getCommand(&cmd,'r'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(56)));}} while(0);

    do {if ((getCommand(&cmd,'\n'))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(57)));}} while(0);

    UnityAssertEqualNumber((UNITY_INT)((APAGAR)), (UNITY_INT)((cmd.cmd)), (

   ((void *)0)

   ), (UNITY_UINT)(58), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((cmd.parametro[0])), (UNITY_INT)((0)), (

   ((void *)0)

   ), (UNITY_UINT)(59), UNITY_DISPLAY_STYLE_INT);

}

void test_apagar_cortado (void){

    do {if (!(getCommand(&cmd,'a'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(62)));}} while(0);

    do {if (!(getCommand(&cmd,'p'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(63)));}} while(0);

    do {if (!(getCommand(&cmd,'a'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(64)));}} while(0);

    do {if (!(getCommand(&cmd,'g'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(65)));}} while(0);

    do {if (!(getCommand(&cmd,' '))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(66)));}} while(0);

    do {if (!(getCommand(&cmd,'a'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(67)));}} while(0);

    do {if (!(getCommand(&cmd,'r'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(68)));}} while(0);

    do {if ((getCommand(&cmd,'\n'))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(69)));}} while(0);

    UnityAssertEqualNumber((UNITY_INT)((SyntaxError)), (UNITY_INT)((cmd.code)), (

   ((void *)0)

   ), (UNITY_UINT)(70), UNITY_DISPLAY_STYLE_INT);

    do {if (((APAGAR) != (cmd.cmd))) {} else {UnityFail( ((" Expected Not-Equal")), (UNITY_UINT)((UNITY_UINT)(71)));}} while(0);

}



void test_IDq_c_espacio_al_final (void){

    do {if (!(getCommand(&cmd,'a'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(75)));}} while(0);

    do {if (!(getCommand(&cmd,'b'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(76)));}} while(0);

    do {if ((getCommand(&cmd,'\n'))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(77)));}} while(0);

    do {if (!(getCommand(&cmd,'i'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(78)));}} while(0);

    do {if (!(getCommand(&cmd,'d'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(79)));}} while(0);

    do {if (!(getCommand(&cmd,'?'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(80)));}} while(0);

    do {if (!(getCommand(&cmd,' '))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(81)));}} while(0);

    do {if ((getCommand(&cmd,'\n'))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(82)));}} while(0);

    UnityAssertEqualNumber((UNITY_INT)((IDq)), (UNITY_INT)((cmd.cmd)), (

   ((void *)0)

   ), (UNITY_UINT)(83), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((cmd.parametro[0])), (UNITY_INT)((0)), (

   ((void *)0)

   ), (UNITY_UINT)(84), UNITY_DISPLAY_STYLE_INT);

}

void test_IDq_c_espacios_al_final (void){

    do {if (!(getCommand(&cmd,'a'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(87)));}} while(0);

    do {if (!(getCommand(&cmd,'b'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(88)));}} while(0);

    do {if ((getCommand(&cmd,'\n'))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(89)));}} while(0);

    do {if (!(getCommand(&cmd,'i'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(90)));}} while(0);

    do {if (!(getCommand(&cmd,'d'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(91)));}} while(0);

    do {if (!(getCommand(&cmd,'?'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(92)));}} while(0);

    do {if (!(getCommand(&cmd,' '))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(93)));}} while(0);

    do {if (!(getCommand(&cmd,' '))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(94)));}} while(0);

    do {if (!(getCommand(&cmd,' '))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(95)));}} while(0);

    do {if ((getCommand(&cmd,'\n'))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(96)));}} while(0);

    UnityAssertEqualNumber((UNITY_INT)((IDq)), (UNITY_INT)((cmd.cmd)), (

   ((void *)0)

   ), (UNITY_UINT)(97), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((cmd.parametro[0])), (UNITY_INT)((0)), (

   ((void *)0)

   ), (UNITY_UINT)(98), UNITY_DISPLAY_STYLE_INT);

}

void test_ID_incompleto (void){

    do {if (!(getCommand(&cmd,'a'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(101)));}} while(0);

    do {if (!(getCommand(&cmd,'b'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(102)));}} while(0);

    do {if ((getCommand(&cmd,'\n'))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(103)));}} while(0);

    UnityAssertEqualNumber((UNITY_INT)((DESCO)), (UNITY_INT)((cmd.cmd)), (

   ((void *)0)

   ), (UNITY_UINT)(104), UNITY_DISPLAY_STYLE_INT);

    do {if (!(getCommand(&cmd,'i'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(105)));}} while(0);

    do {if (!(getCommand(&cmd,'d'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(106)));}} while(0);

    do {if ((getCommand(&cmd,'\n'))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(107)));}} while(0);

    UnityAssertEqualNumber((UNITY_INT)((SyntaxError)), (UNITY_INT)((cmd.code)), (

   ((void *)0)

   ), (UNITY_UINT)(108), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((DESCO)), (UNITY_INT)((cmd.cmd)), (

   ((void *)0)

   ), (UNITY_UINT)(109), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((cmd.parametro[0])), (UNITY_INT)((0)), (

   ((void *)0)

   ), (UNITY_UINT)(110), UNITY_DISPLAY_STYLE_INT);

}

void test_ANG_q (void){

    do {if (!(getCommand(&cmd,'a'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(113)));}} while(0);

    do {if ((getCommand(&cmd,'\n'))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(114)));}} while(0);

    do {if (!(getCommand(&cmd,'a'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(115)));}} while(0);

    do {if (!(getCommand(&cmd,'n'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(116)));}} while(0);

    do {if (!(getCommand(&cmd,'g'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(117)));}} while(0);

    do {if (!(getCommand(&cmd,'?'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(118)));}} while(0);

    do {if ((getCommand(&cmd,'\n'))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(119)));}} while(0);

    UnityAssertEqualNumber((UNITY_INT)((ANGq)), (UNITY_INT)((cmd.cmd)), (

   ((void *)0)

   ), (UNITY_UINT)(120), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((cmd.parametro[0])), (UNITY_INT)((0)), (

   ((void *)0)

   ), (UNITY_UINT)(121), UNITY_DISPLAY_STYLE_INT);

}

void test_ANG_q_c_espacio (void){

    do {if (!(getCommand(&cmd,'a'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(124)));}} while(0);

    do {if ((getCommand(&cmd,'\n'))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(125)));}} while(0);

    do {if (!(getCommand(&cmd,'a'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(126)));}} while(0);

    do {if (!(getCommand(&cmd,'n'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(127)));}} while(0);

    do {if (!(getCommand(&cmd,'g'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(128)));}} while(0);

    do {if (!(getCommand(&cmd,'?'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(129)));}} while(0);

    do {if (!(getCommand(&cmd,' '))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(130)));}} while(0);

    do {if ((getCommand(&cmd,'\n'))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(131)));}} while(0);

    UnityAssertEqualNumber((UNITY_INT)((ANGq)), (UNITY_INT)((cmd.cmd)), (

   ((void *)0)

   ), (UNITY_UINT)(132), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((cmd.parametro[0])), (UNITY_INT)((0)), (

   ((void *)0)

   ), (UNITY_UINT)(133), UNITY_DISPLAY_STYLE_INT);

}

void test_ANG_q_c_numero (void){

    do {if (!(getCommand(&cmd,'a'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(136)));}} while(0);

    do {if ((getCommand(&cmd,'\n'))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(137)));}} while(0);

    do {if (!(getCommand(&cmd,'a'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(138)));}} while(0);

    do {if (!(getCommand(&cmd,'n'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(139)));}} while(0);

    do {if (!(getCommand(&cmd,'g'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(140)));}} while(0);

    do {if (!(getCommand(&cmd,'?'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(141)));}} while(0);

    do {if (!(getCommand(&cmd,' '))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(142)));}} while(0);

    do {if (!(getCommand(&cmd,'1'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(143)));}} while(0);

    do {if (!(getCommand(&cmd,'2'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(144)));}} while(0);

    do {if ((getCommand(&cmd,'\n'))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(145)));}} while(0);

    UnityAssertEqualNumber((UNITY_INT)((SobranParametros)), (UNITY_INT)((cmd.code)), (

   ((void *)0)

   ), (UNITY_UINT)(146), UNITY_DISPLAY_STYLE_INT);

}

void test_ANG_sin_parametro(void){

    do {if ((getCommand(&cmd,'\n'))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(149)));}} while(0);

    do {if (!(getCommand(&cmd,'a'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(150)));}} while(0);

    do {if (!(getCommand(&cmd,'n'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(151)));}} while(0);

    do {if (!(getCommand(&cmd,'g'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(152)));}} while(0);

    do {if ((getCommand(&cmd,'\n'))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(153)));}} while(0);

    UnityAssertEqualNumber((UNITY_INT)((FaltanParametros)), (UNITY_INT)((cmd.code)), (

   ((void *)0)

   ), (UNITY_UINT)(154), UNITY_DISPLAY_STYLE_INT);

}

void test_ANG_Space(void){

    do {if ((getCommand(&cmd,'\n'))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(157)));}} while(0);

    do {if (!(getCommand(&cmd,'a'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(158)));}} while(0);

    do {if (!(getCommand(&cmd,'n'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(159)));}} while(0);

    do {if (!(getCommand(&cmd,'g'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(160)));}} while(0);

    do {if (!(getCommand(&cmd,' '))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(161)));}} while(0);

    do {if ((getCommand(&cmd,'\n'))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(162)));}} while(0);

}

void test_ANG_c_numero (void){

    do {if (!(getCommand(&cmd,'a'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(165)));}} while(0);

    do {if (!(getCommand(&cmd,'n'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(166)));}} while(0);

    do {if (!(getCommand(&cmd,'g'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(167)));}} while(0);

    do {if (!(getCommand(&cmd,' '))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(168)));}} while(0);

    do {if (!(getCommand(&cmd,'1'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(169)));}} while(0);

    do {if (!(getCommand(&cmd,'2'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(170)));}} while(0);

    do {if (!(getCommand(&cmd,'2'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(171)));}} while(0);

    do {if ((getCommand(&cmd,'\n'))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(172)));}} while(0);

    UnityAssertEqualNumber((UNITY_INT)((122)), (UNITY_INT)((cmd.parametro[0])), (

   ((void *)0)

   ), (UNITY_UINT)(173), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((ANG)), (UNITY_INT)((cmd.cmd)), (

   ((void *)0)

   ), (UNITY_UINT)(174), UNITY_DISPLAY_STYLE_INT);

}

void test_ANG_c_numero_c_2_espacios (void){

    do {if (!(getCommand(&cmd,'a'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(177)));}} while(0);

    do {if (!(getCommand(&cmd,'n'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(178)));}} while(0);

    do {if (!(getCommand(&cmd,'g'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(179)));}} while(0);

    do {if (!(getCommand(&cmd,' '))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(180)));}} while(0);

    do {if (!(getCommand(&cmd,' '))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(181)));}} while(0);

    do {if (!(getCommand(&cmd,'1'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(182)));}} while(0);

    do {if (!(getCommand(&cmd,'5'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(183)));}} while(0);

    do {if (!(getCommand(&cmd,'2'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(184)));}} while(0);

    do {if ((getCommand(&cmd,'\n'))) {} else {UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((UNITY_UINT)(185)));}} while(0);

    UnityAssertEqualNumber((UNITY_INT)((152)), (UNITY_INT)((cmd.parametro[0])), (

   ((void *)0)

   ), (UNITY_UINT)(186), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((ANG)), (UNITY_INT)((cmd.cmd)), (

   ((void *)0)

   ), (UNITY_UINT)(187), UNITY_DISPLAY_STYLE_INT);

}void test_ANG_c_numero_c_espacios_al_final (void){

    do {if (!(getCommand(&cmd,'a'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(189)));}} while(0);

    do {if (!(getCommand(&cmd,'n'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(190)));}} while(0);

    do {if (!(getCommand(&cmd,'g'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(191)));}} while(0);

    do {if (!(getCommand(&cmd,' '))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(192)));}} while(0);

    do {if (!(getCommand(&cmd,'1'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(193)));}} while(0);

    do {if (!(getCommand(&cmd,'5'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(194)));}} while(0);

    do {if (!(getCommand(&cmd,'2'))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(195)));}} while(0);

    do {if (!(getCommand(&cmd,' '))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(196)));}} while(0);

    do {if (!(getCommand(&cmd,' '))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(197)));}} while(0);

    do {if (!(getCommand(&cmd,' '))) {} else {UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((UNITY_UINT)(198)));}} while(0);

    getCommand(&cmd,'\n');



    UnityAssertEqualNumber((UNITY_INT)((152)), (UNITY_INT)((cmd.parametro[0])), (

   ((void *)0)

   ), (UNITY_UINT)(201), UNITY_DISPLAY_STYLE_INT);

    UnityAssertEqualNumber((UNITY_INT)((ANG)), (UNITY_INT)((cmd.cmd)), (

   ((void *)0)

   ), (UNITY_UINT)(202), UNITY_DISPLAY_STYLE_INT);

}
