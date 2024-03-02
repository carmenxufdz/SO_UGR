/*
ejercicio7.c
Escribe un programa que acepte como argumentos el nombre de un programa, sus argumentos si
los tiene, y opcionalmente la cadena “bg”. Nuestro programa deberá ejecutar el programa pasado como
primer argumento en foreground si no se especifica la cadena “bg” y en background en caso contrario. Si el
programa tiene argumentos hay que ejecutarlo con éstos.
*/

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    pid_t pid;
    //para fork
    int estado, i, j, tam = 0, bcg = 0;
    //nos creamos un array con argc elementos que será el que tenga los argumentos para execv
    char *nombre;

    //comprobamos que hay argumentos
    if(argc < 2)
    {
        //si no los hay, devolvemos error
        perror("ERROR argumentos.\n El formato es ./ejercicio7 <fichero/ruta> <argumentos>.\n");
        exit(-1);
    }

    //el nombre del fichero es el pasado en argv[1]
    nombre = argv[1];
    //si el último argumento es bg, hay que establecer bcg a true
    if(strcmp(argv[argc-1], "bg") == 0)
    {
        //actualizamos el booleano de background a true
        bcg = 1;
        //establecemos el tamaño a argc - 3, ya que nuestros argumentos son todo argv de
        //tamaño argc menos argv[0] - el ./ejercicio7, argv[1] el nombre, argv[argc-1] - el bg
        tam = argc - 3;
    }
    //si no está especificado el background
    else
    {
        //el tamaño es argc - 1, sin el argv[0] ni argv[1]
        tam = argc - 2;
    }
    
    //argumentos tendrá tamaño tam y contendrá los argumentos para exec
    char *argumentos[tam];

    //guardamos los argumentos en el array
    for(i = 2, j = 0; i < argc && j < tam; i++, j++){
        argumentos[j] = argv[i];
    }

    //el último argumento lo ponemos a un puntero nulo
    argumentos[j] = (char*)0;

    //si hay que ejecutar en background (bcg true)
    //creamos un hijo que ejecute la orden, con lo que estaría ejecutando en background
    if(bcg)
    {
        //creamos un hijo y comprobamos si ha habido error
        if((pid = fork()) < 0)
        {
            perror("ERROR en la creación del hijo.\n");
            exit(-2);
        }
        //si el que ejecuta es el hijo
        else if(pid == 0)
        {
            //el hijo ejecuta la orden y si hay error, imprime
            if(execv(nombre, argumentos) < 0)
            {
                //si ha devuelto un número negativo, ha habido error
                perror("Return no esperado.\n");
                exit(EXIT_FAILURE);
            }
        }
        //el padre espera al hijo
        pid = wait(&estado);
        printf("\nEl hijo %d ha ejecutado el programa en background.\n", pid);
    }
    //si hay que ejecutarlo en background, será el padre el que ejecute el programa
    else
    {
        //el padre ejecuta la orden
        if(execv(nombre, argumentos) < 0)
        {
            //si ha devuelto un número negativo, ha habido error
            perror("Return no esperado.\n");
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}