/*
ejercicio5.c
Implementa un programa que lance cinco procesos hijo. Cada uno de ellos se identificará en la
salida estándar, mostrando un mensaje del tipo Soy el hijo PID. El proceso padre simplemente tendrá que
esperar la finalización de todos sus hijos y cada vez que detecte la finalización de uno de sus hijos escribirá
en la salida estándar un mensaje del tipo:
    Acaba de finalizar mi hijo con <PID>
    Sólo me quedan <NUM_HIJOS> hijos vivos

Implementa una modificación sobre el anterior pragrama en la que el proceso padre espera primero a los hijos 
creados en orden impar y después a los hijos pares
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
    const int HIJOS = 5;
    pid_t pid, hijos[HIJOS];
    int estado, h=4;
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
        else
        {
            hijos[i]=pid;
        }
    }

    //el padre espera a los hijos creados en orden impar
    for(int i = 0; i < HIJOS; i += 2)
    {
        //el padre espera al proceso de pid hijos[i]
        pid = waitpid(hijos[i], &estado, 0);
        printf("\nAcaba de finalizar mi hijo %i\n", pid);
        printf("Me quedan %i hijos vivos\n", h--);
    }
    //el padre espera a los hijos creados en orden par
    for(int i = 1; i < HIJOS; i += 2)
    {
        //el padre espera al proceso de pid hijos[i]
        pid = waitpid(hijos[i], &estado, 0);
        printf("\nAcaba de finalizar mi hijo %i\n", pid);
        printf("Me quedan %i hijos vivos\n", h--);
    }
    return 0;
}