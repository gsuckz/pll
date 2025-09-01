#include "numeros.h"

static bool charToInt(char c, int *num)
{
    if (c <= '9' && c >= '0') {
        *num = (int)c - 48;
        return 1;
    } else {
        return 0;
    }
}

bool agregarDig(Numero *numero, char c)
{
    int num;
    if (numero->index + 1 == 10 || !charToInt(c, &num)) {
        return 0;
    }
    numero->digitos[numero->index] = num;
    numero->index++;
    return 1;
}

int getNumero(Numero *numero)
{
    int resultado = 0, potencia = 1;
    for (int i = numero->index - 1; i >= 0; i--) {
        resultado += numero->digitos[i] * potencia;
        potencia = potencia * 10;
    }
    numero->index      = 0;
    numero->digitos[0] = 0;
    return resultado;
}

uint8_t toString(int numero, char *char_n, uint8_t size)
{
    int max = 0;
    uint8_t temp;
    if (numero == 0) {
        char_n[0] = '0';
        char_n[1] = '\0';
        return 0;
    }
    for (uint8_t i = 0; i <= size; i++) {
        if (numero == 0) {
            max = i - 1;
            break;
        }
        char_n[i] = (char)(numero % 10 + 48);
        numero    = numero / 10;
    }
    for (int i = 0; i <= max / 2; i++) {
        temp            = char_n[i];
        char_n[i]       = char_n[max - i];
        char_n[max - i] = temp;
    }
    char_n[max + 1] = '\0'; // ERROR EXTRAÑO CON max+1
    return max;
}
