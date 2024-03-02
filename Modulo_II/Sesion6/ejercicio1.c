/*
ejercicio1.c 
Implementa un programa que admita t argumentos. El primer argumento será una orden de Linux;
el segundo, uno de los siguientes caracteres “<” o “>”, y el tercero el nombre de un archivo (que puede
existir o no). El programa ejecutará la orden que se especifica como argumento primero e implementará la
redirección especificada por el segundo argumento hacia el archivo indicado en el tercer argumento. Por
ejemplo, si deseamos redireccionar la entrada estándar de sort desde un archivo temporal, ejecutaríamos:

    $> ./mi_programa sort “<” temporal

Nota. El carácter redirección (<) aparece entre comillas dobles para que no los interprete el shell sino que
sea aceptado como un argumento del programa mi_programa.
*/

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/*
Prueba:
- "<" → sort "<" archivo [ordena la informacióń de archivo]
- ">" → ls ">" archivo [guarda el listado del directorio actual en archivo]
*/
int main(int argc, char *argv[ ]){

    //comprobamos que se pasa el número correcto de argumentos
    if(argc < 4 || argc > 4){
        perror("ERROR en argumentos. \n Formato: ./ejercicio1 [orden] ['>' '<'] [archivo]\n");
        exit(-1);
    }

    //abrimos el archivo
    int fd = open(argv[3], O_RDWR);

    //si ha habido error con el archivo
    if( fd < 0){
        perror("ERROR en la apertura del archivo.\n");
        exit(-2);
    }

    //si el segundo argumento es ">" -> salida estándar
    //queremos que la salida de la orden se almacene en archivo
    if(strcmp(argv[2], ">") == 0){
        //cerramos la salida estándar de este proceso
        close(1);

        //redirigimos la salida al fichero
        if(fcntl(fd, F_DUPFD, 1) == -1){
            perror("ERROR en duplicación de la salida. \n");
            exit(-2);
        }
        //ejecutamos la orden
        if( execlp(argv[1], argv[1], (char*)0) < 0){
            perror("Return no esperado. \n");
            exit(EXIT_FAILURE);
        }
    }

    //si es "<"" -> entrada estándar
    //queremos que la orden tome como entrada el archivo
    else if( strcmp(argv[2], "<") == 0){
        //cerramos la entrada estándar del proceso
        close(0);
        //redirigimos la entrada al fichero
        if(fcntl(fd, F_DUPFD, 0) == -1){
            perror("ERROR en duplicación de la entrada. \n");
            exit(-2);
        }
        //ejecutamos la orden
        if( execlp(argv[1], argv[1], (char*)0) < 0){
            perror("Return no esperado. \n");
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}