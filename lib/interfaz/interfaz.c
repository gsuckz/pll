#include "interfaz.h"
#include "comandos.h"
#include "sintetizador.h"
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <ctype.h>
#include <stdint.h>

// Definición de Macros
#define NOTHING 0
#define SELECT_KEY 1
#define LEFT_KEY 2
#define DOWN_KEY 3
#define UP_KEY 4
#define RIGHT_KEY 5

int lcd_key    = 0; // ?
int adc_key_in = 0; // ?

// Variables Utilizadas para la detección de teclas
int sensorPin = A0;
float Y;
int lastKey = 0;
int indice  = 0;
int mean;
int button = 0;


//int getKeyId(int);
//int getKeyId2(int);
//void fn_menu(int, String arr[], byte);
//bool fnSwitch(byte);
//void fn_variacion_frec();
//int getKeyId(int);
//void encontrarMaxMin(int *, int);
//int readValue(int);
//void trueValue(int);
//void textBT(int);

// ############### Definición de la figura Flecha ####################
byte flecha[] = {0b00000, 0b00100, 0b00110, 0b11111, 0b00110, 0b00100, 0b00000, 0b00000};
// Definición de Variables
const int rs = D1, en = D2, d4 = D4, d5 = D5, d6 = D6, d7 = D7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); // Instancia LCD

// ==============Definición de los menus=============
String menu1[] = {"Conectar", "Frecuencia", "Test", "Atras"};
int sizemenu1  = sizeof(menu1) / sizeof(menu1[0]);

String menu2[] = { // solo aparece cuando se selecciona 'Frecuencia'
    "Frec. Actual", "Cambiar Frec.", "Atras"};
int sizemenu2  = sizeof(menu2) / sizeof(menu2[0]);

String menuConectar[] = { // solo aparece cuando se selecciona 'Conectar'
    "Est. Conexion", "Atras"};
int sizemenuConect    = sizeof(menuConectar) / sizeof(menuConectar[0]);

String menuTest[] = { // solo aparece cuando se selecciona 'Test'
    "CP sink", "CP source", "CP disable", "Atras"};
int sizemenuTest  = sizeof(menuTest) / sizeof(menuTest[0]);

// ############################## Mis variables #########################
String linea1, linea2;
int level_menu  = 0; // ubicación entre menu 1 y 2
int level2_menu = 0; // ubicación entre menu 3 y 4
int contador    = 0;
float frec      = 10.6; // [10.6 - 11.8] GHz

boolean select_button = false; // para saber si fue presionado Select.
boolean estado        = false; // me permitirá saber si nos encontramos conectados o NO
int estadoConexion    = 0;     // VER SI SERÁ NECESARIA LA VARIABLE...

// Y    = (float)analogRead(sensorPin) / 10;
// mean = readValue(Y);
// trueValue(mean);

// Pantalla Principal permitira ver los detalles del menu
if (level_menu == 0) {
    lcd.setCursor(0, 0);
    lcd.print("Estado:");
    // estado = false; //llamamos a una funcion para saber si estoy conectado o no
    if (estado == true)
        lcd.print("Conect");
    else
        lcd.print("No conect");
    lcd.setCursor(0, 1);
    if (modoTest == false) {
        lcd.print("Frec:");
        lcd.print(frec, 2);
        lcd.print("Ghz");
    } else {
        lcd.print("Modo Test:");
        lcd.print(nombresTests[testActual]);
    }
    if (select_button == true) {
        level_menu = 1;
        SerialBT.println("A Menu 1");
        fn_menu(contador, menu1, sizemenu1);
        select_button = false;
    }
}
// ############## MENU 1 ################
if (level_menu == 1) {
    if (fnSwitch(sizemenu1)) {               // Se carga el menu
        fn_menu(contador, menu1, sizemenu1); // muestra la posición dentro del menu
    }
    if (select_button == true) { // DUDAS (getKeyId() == SELECT_KEY)
        if (contador == 0) {     // Conectar
            contador = 0;
            fn_menu(contador, menuConectar, sizemenuConect);
            level_menu = 2;
        }
        if (contador == 1) { // Seleccionar Frecuencia
            contador = 0;
            fn_menu(contador, menu2, sizemenu2);
            level_menu = 3;
        }
        if (contador == 2) { // Test
            contador = 0;
            fn_menu(contador, menuTest, sizemenuTest);
            level_menu = 4;
        }
        if (contador == 3) { // Atras
            contador   = 0;
            level_menu = 0;
        }
        select_button = false;
    }
}
// ####### FIN MENU 1 #########################

// ########### MENU 2 ############
if (level_menu == 2) {
    if (fnSwitch(sizemenuConect)) {
        fn_menu(contador, menuConectar, sizemenuConect);
    }
    if (select_button == true) { // obs si select esta en true
        if (contador == 0) {     // op1 Est..  Conexion
            // Aca tengo que llamar a una funcion para realizar la conexion al micro.
            //  Si el micro me responde con un OK mostrare un msj _Conectado! durante 2 seg
            //  Sino mostrare un msj _Error Conexion! durante 2 seg
            //  SerialBT.println("Hola");
            contador = 0;
            // fn_menu(contador,menu3,sizemenu3);

            // level2_menu = 1; // Esto me lleva a otro sub menu tal vez no es necesario...
        }
        if (contador == 1) { // op2  Atras...
            contador = 0;
            // fn_menu(contador,menu3,sizemenu3);
            fn_menu(contador, menu1, sizemenu1);
            level_menu = 1;
            // level2_menu = 2;
        }
        select_button = false;
    }
    // ##################### FIN MENU 2#########################
}
// ######################### MENU 3 #######################
if (level_menu == 3) {
    if (fnSwitch(sizemenu2)) {
        fn_menu(contador, menu2, sizemenu2);
    }
    if (select_button == true) { // Incrementar Decrementar Frecuencia
        if (contador == 0) {     // Muestra Frecuencia Actual
                                 // Hacer algo
            lcd.clear();
            // do{
            //   lcd.setCursor(0,0);
            //   lcd.print("Frec. Actual:");
            //   lcd.setCursor(0,1);
            //   lcd.print(frec);
            //   lcd.print(" Ghz");
            // }while(getKeyId((int)S) != SELECT_KEY);

            contador = 0;

            fn_menu(contador, menu2, sizemenu2);
        }
        if (contador == 1) { // Se modifica la Frecuencia y se la establece.
            // Hacer algo
            lcd.clear();
            // do{
            //   fn_variacion_frec();
            //   lcd.setCursor(0,0);
            //   lcd.print("Frecuencia:");
            //   lcd.setCursor(0,1);
            //   lcd.print(frec);
            //   lcd.print(" Ghz");
            // }while(getKeyId((int)S) != SELECT_KEY);

            contador = 1;
            modoTest = false;
            fn_menu(contador, menu2, sizemenu2);
        }
        if (contador == 2) { // Atras
            contador = 1;
            fn_menu(contador, menu1, sizemenu1);
            level_menu = 1;
        }
        select_button = false;
    }
}
// ######################### MENU 4 #######################
if (level_menu == 4) {
    if (fnSwitch(sizemenuTest)) {
        fn_menu(contador, menuTest, sizemenuTest);
    }
    if (select_button == true) {
        if (contador == 0) {   // Tarea1 = "CP_snk" => testActual
            modoTest   = true; // si se selecciona alguno de los test se activa y me modifica el inicio...
            testActual = CP_snk;
            contador   = 0;
            fn_menu(contador, menuTest, sizemenuTest);
            // level2_menu = 1;
        }
        if (contador == 1) {   // Tarea2 = "CP_src" => testActual
            modoTest   = true; // si se selecciona alguno de los test se activa y me modifica el inicio...
            testActual = CP_src;
            contador   = 1;
            fn_menu(contador, menuTest, sizemenuTest);
            // level2_menu = 2;
        }
        if (contador == 2) {   // Tarea3 = "CP_dis" => testActual
            modoTest   = true; // si se selecciona alguno de los test se activa y me modifica el inicio...
            testActual = CP_dis;
            contador   = 2;
            fn_menu(contador, menuTest, sizemenuTest);
        }
        if (contador == 3) { // Atras
            contador = 0;
            fn_menu(contador, menu1, sizemenu1);
            level_menu = 1;
        }
        select_button = false;
    }
}

delay(5);
}

// Funciones para el Menu LCD
/* uniqueValue: recibe 'value' y te devuelve un único valor si es que tiene varias
                coincidencias ademas llama a la funcion textBT y éste te imprime en
                la consola de BT el valor/tecla presionada
                RECORDAR QUE SOLO USAMOS 3 => select, left, rigth
*/
void uniqueValue(int value)
{
    int currentKey = value;
    if (currentKey != lastKey && currentKey != 0) {
        textBT(currentKey);
        button = currentKey;
    }
    lastKey = currentKey;
}
// put function definitions here:
int readValue(int valor)
{ // Promedio Movil.
    static int valores[16] = {0};
    static int s           = 0;
    static int indice      = 0;

    s               = s + valor - valores[indice];
    valores[indice] = valor;
    indice          = (indice + 1) % 16;
    // Parece estar OK
    return (s / 16);
}
/*
valorVerdadero: Busca obtener un valor estable de la tecla presionada.
                Al tener la Coincidencia llama a uniqueValue para devolver
                un único valor.

*/
void trueValue(int value)
{
    static int previo = 0;
    static int n      = 0;
    int actual;
    actual = getKeyId(value);

    if ((actual == previo)) {
        n++;
    } else {
        n      = 0;
        previo = actual;
    }
    if (n == 15) {
        uniqueValue(actual);
        // textBT(actual);
        n = 0;
    }
}
/*textBT: En función al 'value' te devuelve un valor por consola de BT
 */
void textBT(int value)
{
    switch (value) {
    case NOTHING:
        SerialBT.println("NADA");
        break;
    case SELECT_KEY:
        SerialBT.println("SELECT_KEY");
        select_button = true;
        break;
    case LEFT_KEY:
        SerialBT.println("LEFT_KEY");
        break;
    case DOWN_KEY:
        SerialBT.println("DOWN_KEY");
        break;
    case UP_KEY:
        SerialBT.println("UP_KEY");
        break;
    case RIGHT_KEY:
        SerialBT.println("RIGHT_KEY");
        break;
    default:
        SerialBT.println("NADA");
        break;
    }
}
int getKeyId(int value)
{
    if (value > 120) {
        return 0;
    }
    if (value > 110) {
        return SELECT_KEY;
    }
    if (value >= 100) {
        return LEFT_KEY;
    }
    if (value > 80) {
        return DOWN_KEY;
    }
    if (value >= 53) {
        return UP_KEY;
    }
    if (value < 30) { // modificar el valor esta en funcion al read
        return RIGHT_KEY;
    }
    return 0;
}

// Esta función permite el desplazamiento de los menus
void fn_menu(int pos, String menus[], byte sizemenu)
{
    lcd.clear();
    linea1 = "";
    linea2 = "";

    if ((pos % 2) == 0) {
        lcd.setCursor(0, 0);
        lcd.write(byte(0));
        linea1 = menus[pos];

        if (pos + 1 != sizemenu) {
            linea2 = menus[pos + 1];
        }
    } else {
        linea1 = menus[pos - 1];
        lcd.setCursor(0, 1);
        lcd.write(byte(0));
        linea2 = menus[pos];
    }

    // Escribimos linea
    lcd.setCursor(1, 0);
    lcd.print(linea1);
    lcd.setCursor(1, 1);
    lcd.print(linea2);
}
// Permite el desplazamiento sobre el menu actual en el que nos encontremos.
bool fnSwitch(byte sizemenu)
{
    bool retorno = false;
    int boton    = button;
    if (boton == 0)
        return 0;
    else {
        if (boton == LEFT_KEY || boton == RIGHT_KEY) {
            if (boton == RIGHT_KEY) {
                contador++;
                // delay(250);
            } else if (boton == LEFT_KEY) {
                contador--;
                // delay(250);
            }
            if (contador <= 0) {
                contador = 0;
            }
            if (contador >= sizemenu - 1) {
                contador = sizemenu - 1;
            }
            retorno = true;
        }
        button = NOTHING;
        return retorno;
    }
}
// Produce la Variación de Frecuencia con lim sup e inf
void fn_variacion_frec()
{
    // if(getKeyId((int)S) == RIGHT_KEY){
    //   frec= frec + 0.01;
    //   //(250);
    // }else if(getKeyId((int)S)== LEFT_KEY){
    //   frec = frec - 0.01 ;
    //   //delay(250);
    // }
    // if(frec > 11.8){
    //   frec = 11.8;
    // }
    // if( frec < 10.6){
    //   frec = 10.6;
    // }