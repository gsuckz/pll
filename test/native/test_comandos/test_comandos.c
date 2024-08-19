#include "test_comandos.h"
#include "comandos.h"
#include "numeros.h"

CMD cmd;

void setUp(void){
    TEST_ASSERT_TRUE(getCommand(&cmd,'\n')); 
}
void tearDown(void){
    TEST_ASSERT_TRUE(getCommand(&cmd,'\n')); 
}
void test_Comando_invalido (void){
    TEST_ASSERT_FALSE(getCommand(&cmd,'a'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'r'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'c'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'a'));
    TEST_ASSERT_FALSE(getCommand(&cmd,' '));
    TEST_ASSERT_FALSE(getCommand(&cmd,'a'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'n'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'g'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'?'));
    TEST_ASSERT_TRUE(getCommand(&cmd,'\n'));
    TEST_ASSERT_EQUAL(SyntaxError,cmd.code);
    TEST_ASSERT_FALSE(getCommand(&cmd,'r'));
}
void test_IDq (void){
    TEST_ASSERT_FALSE(getCommand(&cmd,'a'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'b'));
    TEST_ASSERT_TRUE(getCommand(&cmd,'\n')); 
    TEST_ASSERT_FALSE(getCommand(&cmd,'i'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'d'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'?'));
    TEST_ASSERT_TRUE(getCommand(&cmd,'\n'));
    TEST_ASSERT_EQUAL(IDq , cmd.cmd);
    TEST_ASSERT_EQUAL(cmd.parametro[0], 0);
}

void test_comadoreste (void){
    TEST_ASSERT_FALSE(getCommand(&cmd,'r'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'e')); 
    TEST_ASSERT_FALSE(getCommand(&cmd,'s'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'e'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'t'));
    TEST_ASSERT_TRUE(getCommand(&cmd,'\n'));
    TEST_ASSERT_EQUAL(RESET_CMD , cmd.cmd);
    TEST_ASSERT_EQUAL(cmd.parametro[0], 0);
}

void test_apagar (void){
    TEST_ASSERT_FALSE(getCommand(&cmd,'a'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'p'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'a')); 
    TEST_ASSERT_FALSE(getCommand(&cmd,'g'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'a'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'r'));
    TEST_ASSERT_TRUE(getCommand(&cmd,'\n'));
    TEST_ASSERT_EQUAL(APAGAR , cmd.cmd);
    TEST_ASSERT_EQUAL(cmd.parametro[0], 0);
}
void test_apagar_cortado (void){
    TEST_ASSERT_FALSE(getCommand(&cmd,'a'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'p'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'a')); 
    TEST_ASSERT_FALSE(getCommand(&cmd,'g'));
    TEST_ASSERT_FALSE(getCommand(&cmd,' '));
    TEST_ASSERT_FALSE(getCommand(&cmd,'a'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'r'));
    TEST_ASSERT_TRUE(getCommand(&cmd,'\n'));
    TEST_ASSERT_EQUAL(SyntaxError,cmd.code);
    TEST_ASSERT_NOT_EQUAL(APAGAR , cmd.cmd);
}

void test_IDq_c_espacio_al_final (void){
    TEST_ASSERT_FALSE(getCommand(&cmd,'a'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'b'));
    TEST_ASSERT_TRUE(getCommand(&cmd,'\n')); 
    TEST_ASSERT_FALSE(getCommand(&cmd,'i'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'d'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'?'));
    TEST_ASSERT_FALSE(getCommand(&cmd,' '));
    TEST_ASSERT_TRUE(getCommand(&cmd,'\n'));
    TEST_ASSERT_EQUAL(IDq , cmd.cmd);
    TEST_ASSERT_EQUAL(cmd.parametro[0], 0);
}
void test_IDq_c_espacios_al_final (void){
    TEST_ASSERT_FALSE(getCommand(&cmd,'a'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'b'));
    TEST_ASSERT_TRUE(getCommand(&cmd,'\n')); 
    TEST_ASSERT_FALSE(getCommand(&cmd,'i'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'d'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'?'));
    TEST_ASSERT_FALSE(getCommand(&cmd,' '));
    TEST_ASSERT_FALSE(getCommand(&cmd,' '));
    TEST_ASSERT_FALSE(getCommand(&cmd,' '));
    TEST_ASSERT_TRUE(getCommand(&cmd,'\n'));
    TEST_ASSERT_EQUAL(IDq , cmd.cmd);
    TEST_ASSERT_EQUAL(cmd.parametro[0], 0);
}
void test_ID_incompleto (void){
    TEST_ASSERT_FALSE(getCommand(&cmd,'a'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'b'));
    TEST_ASSERT_TRUE(getCommand(&cmd,'\n')); 
    TEST_ASSERT_EQUAL(DESCO, cmd.cmd);
    TEST_ASSERT_FALSE(getCommand(&cmd,'i'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'d'));
    TEST_ASSERT_TRUE(getCommand(&cmd,'\n'));
    TEST_ASSERT_EQUAL(SyntaxError,cmd.code);
    TEST_ASSERT_EQUAL(DESCO, cmd.cmd);
    TEST_ASSERT_EQUAL(cmd.parametro[0], 0);
}
void test_ANG_q (void){
    TEST_ASSERT_FALSE(getCommand(&cmd,'a'));
    TEST_ASSERT_TRUE(getCommand(&cmd,'\n'));   
    TEST_ASSERT_FALSE(getCommand(&cmd,'a'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'n'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'g'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'?'));
    TEST_ASSERT_TRUE(getCommand(&cmd,'\n'));
    TEST_ASSERT_EQUAL(ANGq, cmd.cmd);
    TEST_ASSERT_EQUAL(cmd.parametro[0], 0);
}
void test_ANG_q_c_espacio (void){
    TEST_ASSERT_FALSE(getCommand(&cmd,'a'));
    TEST_ASSERT_TRUE(getCommand(&cmd,'\n'));   
    TEST_ASSERT_FALSE(getCommand(&cmd,'a'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'n'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'g'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'?'));
    TEST_ASSERT_FALSE(getCommand(&cmd,' '));
    TEST_ASSERT_TRUE(getCommand(&cmd,'\n'));
    TEST_ASSERT_EQUAL(ANGq, cmd.cmd);
    TEST_ASSERT_EQUAL(cmd.parametro[0], 0);
}
void test_ANG_q_c_numero (void){
    TEST_ASSERT_FALSE(getCommand(&cmd,'a'));
    TEST_ASSERT_TRUE(getCommand(&cmd,'\n'));   
    TEST_ASSERT_FALSE(getCommand(&cmd,'a'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'n'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'g'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'?'));
    TEST_ASSERT_FALSE(getCommand(&cmd,' '));
    TEST_ASSERT_FALSE(getCommand(&cmd,'1'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'2'));
    TEST_ASSERT_TRUE(getCommand(&cmd,'\n'));
    TEST_ASSERT_EQUAL(SobranParametros,cmd.code);
}
void test_ANG_sin_parametro(void){
    TEST_ASSERT_TRUE(getCommand(&cmd,'\n')); 
    TEST_ASSERT_FALSE(getCommand(&cmd,'a'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'n'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'g'));
    TEST_ASSERT_TRUE(getCommand(&cmd,'\n'));
    TEST_ASSERT_EQUAL(FaltanParametros,cmd.code);
}
void test_ANG_Space(void){
    TEST_ASSERT_TRUE(getCommand(&cmd,'\n')); 
    TEST_ASSERT_FALSE(getCommand(&cmd,'a'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'n'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'g'));
    TEST_ASSERT_FALSE(getCommand(&cmd,' '));
    TEST_ASSERT_TRUE(getCommand(&cmd,'\n'));
}
void test_ANG_c_numero (void){
    TEST_ASSERT_FALSE(getCommand(&cmd,'a'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'n'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'g'));
    TEST_ASSERT_FALSE(getCommand(&cmd,' '));
    TEST_ASSERT_FALSE(getCommand(&cmd,'1'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'2'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'2'));
    TEST_ASSERT_TRUE(getCommand(&cmd,'\n'));
    TEST_ASSERT_EQUAL(122,cmd.parametro[0]);
    TEST_ASSERT_EQUAL(ANG,cmd.cmd);
}
void test_ANG_c_numero_c_2_espacios (void){
    TEST_ASSERT_FALSE(getCommand(&cmd,'a'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'n'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'g'));
    TEST_ASSERT_FALSE(getCommand(&cmd,' '));
    TEST_ASSERT_FALSE(getCommand(&cmd,' '));
    TEST_ASSERT_FALSE(getCommand(&cmd,'1'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'5'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'2'));
    TEST_ASSERT_TRUE(getCommand(&cmd,'\n'));
    TEST_ASSERT_EQUAL(152,cmd.parametro[0]);
    TEST_ASSERT_EQUAL(ANG,cmd.cmd);
}void test_ANG_c_numero_c_espacios_al_final (void){
    TEST_ASSERT_FALSE(getCommand(&cmd,'a'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'n'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'g'));
    TEST_ASSERT_FALSE(getCommand(&cmd,' '));
    TEST_ASSERT_FALSE(getCommand(&cmd,'1'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'5'));
    TEST_ASSERT_FALSE(getCommand(&cmd,'2'));
    TEST_ASSERT_FALSE(getCommand(&cmd,' '));
    TEST_ASSERT_FALSE(getCommand(&cmd,' '));
    TEST_ASSERT_FALSE(getCommand(&cmd,' '));
    getCommand(&cmd,'\n');
    //TEST_ASSERT_TRUE(getCommand(&cmd,'\n'));
    TEST_ASSERT_EQUAL(152,cmd.parametro[0]);
    TEST_ASSERT_EQUAL(ANG,cmd.cmd);
    TEST_ASSERT_EQUAL(OK,cmd.code);
}

