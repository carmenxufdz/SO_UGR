/*
ejercicio3.c
Programa una nueva orden que recorra la jerarquía de subdirectorios existentes a partir de uno
dado como argumento y devuelva la cuenta de todos aquellos archivos regulares que tengan permiso de
ejecución para el grupo y para otros. Además del nombre de los archivos encontrados, deberá devolver sus
números de inodo y la suma total de espacio ocupado por dichos archivos. El formato de la nueva orden
será:
$> ./buscar <pathname>
donde <pathname> especifica la ruta del directorio a partir del cual queremos que empiece a analizar la
estructura del árbol de subdirectorios. En caso de que no se le de argumento, tomará como punto de partida
el directorio actual. Ejemplo de la salida después de ejecutar el programa:
	Los i-nodos son:
	./a.out 55
	./bin/ej 123
	./bin/ej2 87
	...
	Existen 24 archivos regulares con permiso x para grupo y otros
	El tamaño total ocupado por dichos archivos es 2345674 bytes
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

/*
definimos una macro para comprobar si un fichero tiene:
- permiso x para grupos
-> 000010
- permiso x para otros
-> 000001
1. Al modo le quitamos el flag de tipo de fichero (0170000)
2. Le hacemos & 011 (para ver si tiene activado los permisos x de GRP (010) y OTH(011))
(GRP & OTH = 011)
3. Si el resultado es 011, tiene los permisos x activados para GRP y OTH
Ejemplo: st_mode = 100675 (en octal)
st_mode: 100675 -> 001 000 000 110 111 101 en binario
S_IFMT: 170000 -> 001 111 000 000 000 000 en binario
~S_IFMT:
-> 110 000 111 111 111 111
Por tanto, st_mode & ~ S_IFMT:
001 000 000 110 111 101
& 110 000 111 111 111 111
---------------------------------------
000 000 000 110 111 101
u g o
A esta altura, nos hemos quedado con los permisos para user, group y other
El permiso x de other es (0000001):
El permiso x de group es (0000010):
000 000 000 000 000 001
000 000 000 000 000 010
La suma de estos dos es 000011(octal), por tanto, volviendo al resultado de st_mode & ~S_IFMT, le
hacemos & 011:
000 000 000 110 111 101
& 000 000 000 000 001 001
-------------------------------------
000 000 000 000 001 001 -> en octal es 000011
Al ser el resultado 011 en octal, indica que el fichero tiene permisos x para gr y oth; en caso que no
saliera 011, supondría que dichos bits no están activados, por tanto, no tendría permisos x para gr y oth.
*/
#define permisos(mode) (((mode & ~S_IFMT) & 011) == 011)
/*
estructura para almacenar:
- el número de bytes totales
- el número de archivos regulares
*/
struct salida
{
	int bytes;
	int n;
};
/*función para recorrer el subárbol
- recibe como argumento la ruta/nombre del directorio a recorrer
- devuelve la estructura salida con:
el número de archivos regulares con permiso x en gr y oth
el número de bytes totales de los archivos regulares con permiso x en gr y oth*/

struct salida buscar(char nom_dir[1000])
{
	DIR *directorio;
	struct dirent *arch;
	char pathname[1000];
	struct stat atributos;
	struct salida s; s.bytes = 0;
	s.n = 0;

	//abrimos el directorio
	directorio = opendir(nom_dir);

	//comprobamos si ha habido error
	if(directorio == NULL)
	{
		printf("ERROR en la apertura del directorio %s", nom_dir);
		exit(-2);
	}

	//leemos el directorio hasta llegar al final
	while((arch = readdir(directorio)) != NULL)
	{
		//si es el directorio padre o actual, ignoramos
		if(strcmp(arch->d_name, ".") != 0 && strcmp(arch->d_name, "..") != 0)
		{
			//almacenamos la ruta con el nombre de la carpeta y el archivo en pathname
			//de forma que quedaría carpeta/archivo
			sprintf(pathname, "%s/%s", nom_dir, arch->d_name);

			//obtenemos los atributos del archivo: si hay error, lo mostramos por pantalla
			if(stat(pathname, &atributos) < 0)
			{
				printf("ERROR en la obtención de los atributos de %s - %s\n", pathname,
				strerror(errno));
			}
			//si no ha habido problema al obtener los atributos
			else
			{
				//si es un directorio
				if(S_ISDIR(atributos.st_mode))
				{
					//volvemos a llamar a buscar
					s = buscar(pathname);
				}
				//si es un archivo regular y tiene permisos de ejecución para grupo y otro
				else if(S_ISREG(atributos.st_mode) && permisos(atributos.st_mode))
				{
					//Imprimimos el nombre y el número de inodo
					printf("\t%s %li\n", pathname, atributos.st_ino);
					//sumamos el tamaño del archivo
					s.bytes += atributos.st_size;
					//aumentamos el número de archivos regulares
					s.n += 1;
				}
			}
		}
	}
	return s;
}

int main(int argc, char *argv[])
{
	char nom_dir[1000];
	struct salida s;

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

	//recorremos el directorio con la función buscar; la función devuelve el número de bytes totales
	s = buscar(nom_dir);

	//imprimimos el número de archivos regulares y el tamaño total en bytes
	printf("\nExisten %i archivos regulares con permiso x para grupos y otros.\n", s.n);
	printf("El tamaño total ocupado por dichos archivos es %i bytes.\n", s.bytes);
	return 0;
}