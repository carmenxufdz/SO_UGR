/*
Ejercicio 2.
Reescribir el programa que implemente un encauzamiento de dos órdenes pero utilizando fcntl.
Este programa admitirá tres argumentos. El primer argumento y el tercero serán dos órdenes de Linux. El
segundo argumento será el carácter “|”. El programa deberá ahora hacer la redirección de la salida de la
orden indicada por el primer argumento hacia el cauce, y redireccionar la entrada estándar de la segunda
orden desde el cauce. Por ejemplo, para simular el encauzamiento ls|sort, ejecutaríamos nuestro programa
como:

    $> ./mi_programa2 ls “|” sort
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char *argv[ ]){
    int fd[2]; pid_t PID;
    //comprobamos que se pasa el número correcto de argumentos
    if(argc < 4 || argc > 4){
        perror("ERROR en argumentos. \n Formato: ./ejercicio1 [orden] ['|'] [orden]\n");
        exit(-1);
    }
    //comprobamos que el tercer argumento es "|"
    if(strcmp(argv[2], "|") != 0){
        perror("ERROR en el 3er argumento, debe ser '|'.\n");
        exit(-1);
    }
    //el padre crea un cauce
    //crea un hijo y comprueba si ha habido error
    if( (PID = fork()) < 0){
        perror("ERROR en la creación del hijo.\n");
        exit(-1);
    }
    //si es el hijo el que ejecuta, va a realizar la primera orden
    if(PID == 0){
        close(fd[0]); //como el hijo es el que escribe, cerramos el descriptor de lectura del cauce
        close(1);
        //cerramos la salida estándar
        //y la redirigimos la salida al pipe
        if(fcntl(fd[1], F_DUPFD, 1) < 0){
            perror("ERROR en duplicación de la salida. \n");
            exit(-2);
        }
        printf(" Este print no debería salir%i\n");
        //ejecutamos la orden
        if( execlp(argv[1], argv[1], (char*)0) < 0){
            perror("Return no esperado. \n");
            exit(EXIT_FAILURE);
        }
    }
    //si es el padre, va a realizar la segunda orden
    else{
        close(fd[1]);
        //como el padre es el que lee, cerramos el descriptor de escritura del cauce
        close(0);
        //cerramos la entrada estándar
        //y la redirigimos la entrada al pipe
        if(fcntl(fd[0], F_DUPFD, 0) == -1){
            perror("ERROR en duplicación de la salida. \n");
            exit(-2);
        }
        //ejecutamos la orden
        if( execlp(argv[3], argv[3], (char*)0) < 0){
            perror("Return no esperado x2. \n");
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}