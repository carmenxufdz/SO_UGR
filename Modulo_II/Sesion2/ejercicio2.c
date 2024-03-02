/*
ejercicio 2
Realiza un programa en C utilizando las llamadas al sistema necesarias que acepte como entrada:
    Un argumento que representa el 'pathname' de un directorio
    Otro argumento que es un número octal de 4 dígitos (similar al que se puede utilizar para
    cambiar los permisos en la llamada al sistema chmod). Para convertir este argumento tipo cadena
    a un tipo numérico puedes usar la función strtol.
El programa tiene que usar el número octal indicado en el segundo argumento para cambiar los 
permisos de todos los archivos que se encuentren en el directorio indicado en el primer argumento.
El programa debe proporcionar en la salida estándar una línea para cada archivo del directorio que esté 
formada por:
    <nombre_de_archivo>: <permisos_antiguos> <permisos_nuevos>
Si no se puede cambiar los permisos de un determinado archivo se debe especificar la siguiente información
en la línea de salida:
    <nombre_de_archivo>: <errno> <permisos_antiguos> 
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

int main(int argc, char* argv[])
{
    char *nom_dir; //argumento 1, nombre del directorio
    int permisos; //argumento 2, permisos
    DIR *directorio; //directorio de nombre nom_dir
    struct dirent *arch; //cada entrada del directorio nom_dir
    char pathname[1000]; //nombre de cada entrada del directorio
    struct stat atributos; //atributos de pathname
    mode_t perm_arch; //permisos antiguos de cada entrada del directorio

    if(argc!=3)
    {
        printf("ERROR en argumentos.\nSintaxis: ./ejercicio2 <pathname_directorio> <octal_4_dígitos>");
        exit(EXIT_FAILURE);
    }
    else
    {
        nom_dir = argv[1];
        permisos = strtol(argv[2], NULL, 8);
        if((directorio = opendir(nom_dir)) == NULL)
        {
            printf("Error al abrir el directorio %s", nom_dir);
            exit(-2);
        }

        // Leemos mientras no haya error o no llegue al final del stream del directorio
        while((arch=readdir(directorio))!=NULL)
        {
            // Si es el directorio padre o actual, ignoramos
            if(strcmp(arch->d_name,".")!=0 && strcmp(arch->d_name,"..")!=0)
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
                else
                {
                    perm_arch = atributos.st_mode;

                    // Modificamos los permisos de pathname
                    if(chmod(pathname,(atributos.st_mode & permisos) | S_ISUID)<0)
                    {
                        //si hay error, mostramos el nombre + error + permisos antiguos
                        printf("%s : %o %s \n", arch->d_name, perm_arch, strerror(errno));
                    }
                    else
                    {
                        //obtenemos los atributos actualizaodos
                        stat(pathname, &atributos);
                        //si no ha habido error, mostramos: nombre + permisos antiguos + permisos nuevos
                        printf("%s : %o %o\n", arch->d_name, perm_arch,atributos.st_mode);
                    }
                }
            }
        }
    } 
    return 0;
}