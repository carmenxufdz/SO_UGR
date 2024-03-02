/*
ejercicio4.c
Implementa de nuevo el programa buscar del ejercicio 3 utilizando la llamada al sistema nftw.
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
#include <string.h>
#include <ftw.h>

#define permisos(mode) (((mode & ~S_IFMT) & 011) == 011)
int BYTES = 0;
int N = 0;

//función para buscar archivos regulares
int buscar(const char *path, const struct stat* stat, int flags, struct FTW* ftw)
{
	//ignoramos si es el fichero actual o el padre
	if(strcmp(path, ".") != 0 && strcmp(path, "..") != 0)
	{
		//si es un archivo regular con permisos de ejecución para grupos y otros
		if(S_ISREG(stat->st_mode) && permisos(stat->st_mode))
		{
			//Imprimimos el nombre y el número de inodo
			printf("\t%s %li\n", path, stat->st_ino);
			//sumamos el tamaño del archivo
			BYTES += stat->st_size;
			//aumentamos el número de archivos regulares
			N += 1;
		}
	}
	return 0;
}

int main(int argc, char *argv[])
{
	char nom_dir[1000];
	//si se especifica la ruta ./buscar <ruta>
	if(argc == 2)
	{
		//copiamos el nombre del directorio en nom_dir
		strcpy(nom_dir, argv[1]);
	}
	//si no se especifica la ruta
	else if(argc == 1)
	{
		//copiamos la ruta actual en nom_dir
		strcpy(nom_dir, ".");
	}
	//en cualquier otro caso, error
	else
	{
		printf("ERROR en argumentos.\n El formato es: ./buscar <ruta> ó ./buscar");
		exit(-1);
	}
	//imprimimos el mensaje
	printf("Los i-nodos son:\n");

	//función para recorrer el árbol
	if(nftw(nom_dir, buscar, 5, 0) != 0)
	{
		printf("ERROR en nftw");
		exit(-2);
	}
	
	//imprimimos el número de archivos regulares y el tamaño total en bytes
	printf("\nExisten %i archivos regulares con permiso x para grupos y otros.\n", N);
	printf("El tamaño total ocupado por dichos archivos es %i bytes.\n", BYTES);
	return 0;
}

