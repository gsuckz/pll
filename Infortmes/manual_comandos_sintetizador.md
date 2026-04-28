# Manual de Usuario - Sintetizador de Banda Ku

Este manual describe el funcionamiento y los comandos disponibles para operar el sintetizador de frecuencias en banda Ku (10.6 GHz a 11.8 GHz) a través de un terminal por puerto serie o Bluetooth.

## Conexión y Configuración

El sintetizador permite la comunicación inalámbrica a través de Bluetooth o mediante conexión serie tradicional.
- **Dispositivo Bluetooth:** "Sintetizador de Banda Ku"
- **Baudrate:** 9600 bps

## Formato de los Comandos

Los comandos no son sensibles a mayúsculas o minúsculas (`FREC` es igual a `frec`).
Cada comando y sus respectivos parámetros (si los tiene) deben ir separados por espacios. El comando se ejecutará al enviar un carácter de terminación (`\n` o al presionar Enter).

---

## Lista de Comandos

### `frec` o `frecuencia`
Fija una nueva frecuencia de salida constante en el sintetizador.
- **Parámetros:** 1 (Frecuencia en MHz).
- **Rango permitido:** 10600 a 11800.
- **Ejemplo:** `frec 11000` (Ajusta la frecuencia a 11.0 GHz).
- **Respuesta:** `Frecuencia fijada en: 11000 MHz`

### `frec?` o `frecuencia?`
Consulta la frecuencia configurada actualmente.
- **Parámetros:** 0.
- **Ejemplo:** `frec?`
- **Respuesta:** `Frecuencia fijada en: <frecuencia> MHz`

### `barrer`
Inicia un barrido de frecuencias continuo entre un límite inferior y uno superior, en un tiempo determinado.
- **Parámetros:** 3
  1. `tiempo_ms`: Duración total del barrido en milisegundos (Mínimo: 100).
  2. `f_min`: Frecuencia inicial en MHz (Rango: 10600 - 11800).
  3. `f_max`: Frecuencia final en MHz (Rango: 10600 - 11800).
- **Restricción:** La diferencia entre la frecuencia máxima y mínima debe ser de al menos 100 MHz.
- **Ejemplo:** `barrer 500 10600 11000` (Realiza un barrido desde 10.6 GHz hasta 11.0 GHz en 500 ms).

### `stop`
Detiene un barrido de frecuencias en curso. La frecuencia quedará fijada en el último valor alcanzado al momento de detenerlo.
- **Parámetros:** 0.
- **Ejemplo:** `stop`

### `estado`
Consulta el estado de enclavamiento (Lock) del lazo de enganche de fase (PLL) y el estado de la alimentación.
- **Parámetros:** 0.
- **Ejemplo:** `estado`
- **Respuestas posibles:**
  - `Estado: Enclavado!`
  - `Estado: No Enclavado`
  - `Estado: Error de Alimentación`

### `id?`
Retorna el identificador y la versión del firmware del equipo.
- **Parámetros:** 0.
- **Ejemplo:** `id?`
- **Respuesta:** `Sintetizador de Frecuencias en Banda Ku v0.1`

### Modos Especiales y de Prueba
Estos comandos alteran el comportamiento interno del sintetizador para tareas de calibración o testeo.

- **`iniciar`**: Reinicializa el sintetizador y lo devuelve a su modo de funcionamiento NORMAL.
- **`cpdis`**: Deshabilita la bomba de carga (Charge Pump Disabled).
- **`cpsnk`**: Fuerza la bomba de carga en modo sumidero (Charge Pump Sink).
- **`cpsrc`**: Fuerza la bomba de carga en modo fuente (Charge Pump Source).
- **`cft`**: Conecta la salida del comparador de fase al pin de Test.
