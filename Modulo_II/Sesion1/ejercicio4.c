/*
ejercicio4.c
Define una macro en lenguaje C que tenga la misma funcinalidad que la macro S_ISREG(mode)
usando para ello los flags definidos en <sys/stat.h> para el campo st_mode de la struct stat,
y comprueba que funciona en un programa simple. Consulta en un libro de C o en internet como
se especifica una macro con argumento en C
*/

#include<unistd.h>  /* POSIX Standard: 2.10 Symbolic Constants <unistd.h>
		     */
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>  /* Primitive system data types for abstraction	\
			   of implementation-dependent data types.
			   POSIX Standard: 2.6 Primitive System Data Types
			   <sys/types.h>
		       */
#include<sys/stat.h>
#include<stdio.h>
#include<errno.h>
#include<string.h>

// MACRO
#define S_ISREG2(mode) ((mode & S_IFMT)==S_IFREG)
#define S_ISDIR2(mode) ((mode & S_IFMT)==S_IFDIR)

int main(int argc, char *argv[])
{
    if(argc<2){
        printf("\nSintaxis de ejecucion: ejercicio4 [<nombre_archivo>]+\n\n");
        exit(EXIT_FAILURE);
    }

    struct stat atributos;
    char *pathname;
    for (int i = 1; i < argc; ++i)
    {
        if(lstat(argv[i], &atributos)<0){
            printf("\nError al intentar acceder a los atributos de %s",argv[i]);
            perror("\nError en lstat");
        }else{
            if (S_ISREG2(atributos.st_mode))
                printf("%s : REGULAR \n", argv[i]);
            else if(S_ISDIR2(atributos.st_mode))
                printf("%s : DIRECTORIO\n", argv[i]);
    }
    }
    return 0;

}