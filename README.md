# eiv23-TP5

que posicion adopta el motor sin señal?
Palabras a usar?
echo 
ang <45° 7´ 45´´>
ang <45,5°>



Modulo UART
Modulo TIMER
Maquina de estado de comandos--


Syntax:
COMmand(?) <parameter>/n 

Comandos:
ANGle: Angulo del servo
ANGLE_FULL: Rota entre 0 y 180, movimiento de vaivén //?

_ :
? Consulta el angulo actual del servo:

EJ:
ANG 45  -- fija el angulo a 45°
ANG? -- Envía: El angulo fijado es 45° 

Si existen varios servos:

SERVO1/ANG?
SERVO2/ANG 40

Si no se especifica, el ultimo periferico modificado es el que se cambia ej:
SERVO1/ANG 25
ANG? -- Devuelve el angulo de Servo1

SERVO1/ANG?
ANG 40 
SERVO2/ANG?










