/*
ejercicio1.c
El programa acepta como argumento el nombre de un archivo (pathname), lo abre 
y lo lee en bloques de tamaño de 80 bytes. y crea un archivo de salida, salida.txt, 
en el que debe aparecer la siguiente información

Modificación
El número de bloques es <n bloques>
Bloque 1
// Primeros 80 bytes del archivo
Bloque 2
// Los siguientes 80 bytes del archivo
...
Bloque n
// Los siguientes 80 bytes del archivo
Si no se pasa un argumento al programa se debe utilizar la entrada estándar como archivo de entrada
*/
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>  	/* Primitive system data types for abstraction\
			   of implementation-dependent data types.
			   POSIX Standard: 2.6 Primitive System Data Types
			   <sys/types.h>
			*/
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include<string.h>

int main(int argc, char *argv[])
{
    int entrada, salida, contador;
    // guardamos el bloque n para escribirlo en el fichero
    char bloque[10];
    // bytes leidos/escritos
    int bytes=80;
    // buffer del que vamos a leer y escribir
    char buffer[81];

    char bloques[30]="El numero de bloques es 100";

    if(argc==2)
    {
        entrada = open(argv[1], O_RDONLY);
    }

    else
    {
        entrada = STDIN_FILENO; // Entrada estandar, como cin
    }

    if(entrada<0)
    {
        printf("\nError %d en open", errno);
        perror("\nError en open");
        exit(-1);
    }

    if((salida=open("salida.txt", O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR))<0)
    {
        printf("\nError %d en open", errno);
        perror("\nError en open");
        exit(-2);
    }
    
    // Hacemos espacio para escribir la modificación al principio del fichero, por lo que
    // avanzamos el current file offset
    lseek(salida, sizeof(bloques), SEEK_SET);
    write(salida, "\n", sizeof("\n"));

    for(int i=0; read(entrada,buffer,bytes); i++)
    {
        // Escribir Bloque n
        sprintf(bloque, "Bloque %d", i+1);
        write(salida, bloque, sizeof(bloque));

        // Escribir el salto de linea
        write(salida, "\n", sizeof("\n"));

        //Escribir lo leído en la entrada
        write(salida, buffer, bytes);

        // Escribir salto de linea
        write(salida, "\n", sizeof("\n"));

        memset(buffer, 0, 81);

        contador = i+1;
    }

    // Nos posicionamos al principio del fichero
    lseek(salida, 0, SEEK_SET);

    sprintf(bloques, "El numero de bloques es %d", contador);
    write(salida, bloques, sizeof(bloques));
    
    close(entrada);
    close(salida);

    return 0;
}