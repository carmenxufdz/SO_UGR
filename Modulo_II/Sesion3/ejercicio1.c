/*
ejercicio1.c
Implementa un programa en C que tenga como argumento un número entero. Este programa debe
crear un proceso hijo que se encargará de comprobar si dicho número es un número par o impar e informará
al usuario con un mensaje que se enviará por la salida estándar. A su vez, el proceso padre comprobará si
dicho número es divisible por 4, e informará si lo es o no usando igualmente la salida estándar.
*/


#include<sys/types.h>	//Primitive system data types for abstraction of implementation-dependent data types.
						//POSIX Standard: 2.6 Primitive System Data Types <sys/types.h>
#include<unistd.h>		//POSIX Standard: 2.10 Symbolic Constants         <unistd.h>
#include<sys/stat.h>
#include<fcntl.h>		//Needed for open
#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#include<dirent.h>
#include<string.h>

int main(int argc, char *argv[])
{
    pid_t pid;
    int num;

    if(argc!=2)
    {
        printf("ERROR en argumentos.\nSintaxis: ./ejercicio3 <num>");
        exit(EXIT_FAILURE);
    }

    num = atoi(argv[1]);
    
    // creamos un proceso hijo y comprobamos que no ha habido error
    if( (pid = fork()) < 0)
    {
        perror("Error en el fork\n");
        exit(-1);
    }

    // si el que ejecuta este trozo de código es el hijo
    else if(pid == 0) 
    {
        //comprobamos si el número es par o impar y lo mostramos por pantalla
        if(num%2 == 0)
        {
            printf("H: El número %i es par.\n", num);
        }
        else
        {
            printf("H: El número %i es impar.\n", num);
        }
    }
    // si el que ejecuta este código es el padre
    else
    {
        //comprobamos si el número es divisible por 4
        if(num%4 == 0)
        {
            printf("P: El número %i es divisible por 4.\n", num);
        }
        else
        {
            printf("P: El número %i no es divisible por 4.\n", num);
        }
    }
exit(0);
}