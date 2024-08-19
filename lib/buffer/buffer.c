#include "buffer.h"
static uint8_t siguienteModular(uint8_t valor,uint8_t modulo){//quepasa si llega aca la int??0.
    if (!modulo) return 0;
    if (valor >= (modulo - 1)) {
        valor = 0;
    }else{
        valor = valor + 1;
    }
    return valor;
}

static bool BufferLleno(Buffer * self){
    //return (self->i_save + 1) == self->i_use;
    return siguienteModular(self->i_save,BUFFER_MAX) == self->i_use;
}

static bool BufferVacio(Buffer * self){
    return self->i_use == self->i_save;
}


bool buffer_escribir(Buffer * self, char caracter){
    if (BufferLleno (self)) return 0;//que pasa si llega aca la interrupcion?
    self->chars[self->i_save] = caracter;
    // Asignar nuevo valor
    self->i_save = siguienteModular(self->i_save,BUFFER_MAX);
    return true;
}


bool buffer_leer(Buffer * self, char * caracter_p){
    if (BufferVacio(self)) return 0;

    *caracter_p = self->chars[self->i_use];
    self->i_use = siguienteModular(self->i_use,BUFFER_MAX);
    return 1;
}

void buffer_clr(Buffer * self){
    self->i_use = self->i_save;
}


