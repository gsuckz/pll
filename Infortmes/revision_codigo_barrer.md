# Revisión de Código: Función de Barrido (`barrer`)

Tras analizar en profundidad el código del parser de comandos (`comandos.c`) y la lógica del sintetizador (`sintetizador.cpp`), he detectado **errores críticos en la función de barrido** que impedirán su correcto funcionamiento en la mayoría de los casos prácticos.

A continuación, se detalla por qué la función `barrer` fallará y las razones matemáticas de los bugs.

## 1. Conflicto de Límites (Siempre arrojará error de Buffer)

Existe una contradicción entre las validaciones del comando y la capacidad del buffer, lo que hace que el barrido casi siempre aborte.

**En `comandos.c`:**
Se obliga a que la diferencia entre la frecuencia máxima y mínima sea de al menos 100 MHz:
```c
if (cmd->parametro[1] >= (cmd->parametro[2] + 100)) { // Falla si max - min < 100
```

**En `sintetizador.cpp` (`configurarBarrido`):**
Se calcula la cantidad de pasos de frecuencia basándose en un paso mínimo de 1 MHz:
```c
numeroPasosf = (frecuenciaBarridoMax - frecuenciaBarridoMin) / FRECUENCIA_PASO_MIN; 
// Como mínimo, esto será 100.
```
Luego, se toma el máximo entre `numeroPasosf` y los pasos por tiempo (`numeroPasosT`), por lo que `numeroPasos` **siempre será mayor o igual a 100**.

El problema ocurre en la siguiente validación:
```c
if (numeroPasos > TAMANO_BUFFER_BARRIDO / 2) { 
    SerialBT.println("ERROR: Numero de pasos excede el tamaño del buffer de barrido");
    return;
}
```
Como `TAMANO_BUFFER_BARRIDO` es 200, el límite máximo permitido de pasos es 100.
**Consecuencia:** Si el usuario intenta hacer un barrido de 10.6 GHz a 11.0 GHz (diferencia de 400 MHz), `numeroPasos` será al menos 400. La validación fallará, arrojará error y el barrido **no se ejecutará**. La única forma en que funcione es si se piden *exactamente* 100 MHz de diferencia y un tiempo menor o igual a 100 ms.

---

## 2. División Entera (El barrido se queda estancado en la misma frecuencia)

Incluso si sorteáramos el problema del buffer (aumentando su tamaño), el barrido fallará en cambiar las frecuencias debido a un error de tipos de datos.

El paso de frecuencia se calcula así (usando enteros):
```c
pasoFrecuencia = ((frecuenciaBarridoMax - frecuenciaBarridoMin) / numeroPasos);
```
Si el usuario pide hacer un barrido de 100 MHz en 500 ms, entonces `numeroPasos` se define por el tiempo y vale 500.
Al calcular: `100 / 500`, **el resultado en división entera en C/C++ es `0`**.

Más abajo, en el bucle que llena el buffer:
```c
temp += pasoFrecuencia; 
```
Como `pasoFrecuencia` es 0, a `temp` se le suma 0 en cada iteración.
**Consecuencia:** El buffer se llenará con la frecuencia inicial repetida 500 veces. El sintetizador hará "pasos" en el tiempo, pero la frecuencia se mantendrá constante, sin barrer realmente.

---

## 3. Inconsistencia de Unidades en `numeroPasosT`

En la función se calcula el número de pasos por tiempo de esta manera:
```c
numeroPasosT = duracion / FRECUENCIA_PASO_MIN; // FRECUENCIA_PASO_MIN = 1
```
Se está dividiendo un tiempo (milisegundos) por una constante que representa Frecuencia (MHz). Esto carece de sentido físico. Lo correcto sería definir un "tiempo de paso mínimo" (`TIEMPO_PASO_MIN_MS`) y usarlo para calcular cuántos pasos temporales caben en la duración total.

---

## Conclusión

Actualmente, la función `barrer` **no es funcional** para la mayoría de los escenarios. 

### Soluciones Propuestas:
1. **Aumentar drásticamente el tamaño del buffer:** Si se desea permitir barridos de hasta 1200 MHz (10.6 a 11.8), `TAMANO_BUFFER_BARRIDO` debería ser al menos de 2400. Sin embargo, esto consumirá mucha memoria RAM en el microcontrolador.
2. **Generación al Vuelo (Sin Buffer):** En lugar de precalcular todas las frecuencias en un buffer gigante, se debería calcular y enviar la frecuencia deseada directamente en cada ciclo de la función `SintetizadorTickBarrido()`.
3. **Usar `float` para el paso de frecuencia:** `pasoFrecuencia` y `temp` deberían ser variables de punto flotante para acumular fracciones de MHz cuando los pasos de tiempo sean mayores que la diferencia de frecuencia, enviando el valor redondeado al sintetizador.
