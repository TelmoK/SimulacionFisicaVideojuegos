# SimulacionFisicaVideojuegos
Repositorio de las prácticas de la asignatura de Simulación Física de Videojuegos

En la carpeta skeleton estará vuestro código fuente

Necesitaréis además descomprimir el archivo que se indica a continuación en la raíz del repositorio:

Carpetas bin y common https://ucomplutense-my.sharepoint.com/:f:/g/personal/liagar05_ucm_es/ElNxHPmZVj9Ni9-8FKTKp7cBJHAarnL3vvEvG50z0QMrzg

## Instrucciones de uso

Los controles no son de lo más cómodo ni intuitivo por cuestiones de tiempo pero básicamente podemos hacer que el submarino se hunda con la techa K o que flote con la L. Teóricamente lo que se está haciendo por debajo es llenar o vaciar los tanques de lastre del submarino modificando así su masa total (ver fillAirTank() en Submarine.cpp).
Con la R se cambia la dirección del timón.

Con el 1 se cambia a primera persona desde el submarino, con el 2 de cambia a un “plano selfie”, con el 3 a una vista cenital un poco por detrás del submarino, y con el 4 la cámara se desprende del submarino y se maneja con las teclas WASD.
Con la P lanzamos un proyectil solo si estamos en primera persona (para ello presionar tecla 1), y con la T se lanza un proyectil hacia arriba desde el centro de la nave, este proyectil es más pesado.

Adicionalmente el usuario puede descomentar la sección “DEBUG LOGS” en la función applyMotorForce() de Submarine.cpp para así poder ver los valores de la posición y demás del submarino, aunque esto hará más lento el programa y puede producir errores en caso de cargar mucho la consola.
