/*
ejercicio4.c
Implementa un programa que lance cinco procesos hijo. Cada uno de ellos se identificará en la
salida estándar, mostrando un mensaje del tipo Soy el hijo PID. El proceso padre simplemente tendrá que
esperar la finalización de todos sus hijos y cada vez que detecte la finalización de uno de sus hijos escribirá
en la salida estándar un mensaje del tipo:
    Acaba de finalizar mi hijo con <PID>
    Sólo me quedan <NUM_HIJOS> hijos vivos
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
int main()
{
    pid_t pid;
    int estado;
    const int HIJOS = 5;
    //creación de los hijos
    for(int i = 0; i < HIJOS; i++)
    {
        //si ha habido error, salimos del programa
        if((pid = fork()) < 0)
        {
            perror("ERROR al crear hijo\n");
            exit(-1);
        }
        //si se ha creado el proceso y es el hijo el que ejecuta
        else if(pid == 0)
        {
        //muestra su PID por pantalla
        printf("\nSoy el hijo %i\n", getpid());
        exit(0); //se finaliza el proceso hijo
        }
    }

    //el padre espera a los hijos
    for(int i = 0; i < HIJOS; i++)
    {
        //espero a un proceso hijo
        pid = wait(&estado);
        printf("\nAcaba de finalizar mi hijo %i\n", pid);
        printf("Me quedan %i hijos vivos\n", HIJOS - i - 1);
    }
    return 0;
}