
#ifndef INTERFAZ_H
#define INTERFAZ_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    NOTHING     = 0,
    SELECT_KEY  = 1,
    LEFT_KEY    = 2,
    DOWN_KEY    = 3,
    UP_KEY      = 4,
    RIGHT_KEY   = 5
} codigoTecla;

typedef struct teclado{
    int cola[10];   
    int cursor;

}teclado;

int actualizarTeclado(void);
int leerCola(void);
int generarTeclado(void);


#ifdef __cplusplus
}
#endif

#endif

/*
Table value AnalogRead A0
Select : 1190-1150
Left : 1070 - 1030
Down: 900 - 860
Up: 630 - 550
Rigth: 10 - 0

Table Value AnalogRead A0 with charger 5V
Select : 1270-1255
Left : 1170 - 1130
Down: 970 - 950
Up:  670- 625
Rigth: 30 - 0
*/