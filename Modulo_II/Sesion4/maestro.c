/*
maestro.c
El intervalo de números naturales donde calcular los número primos se pasará como argumento al
programa maestro. El maestro creará dos procesos esclavos y dividirá el intervalo en dos
subintervalos de igual tamaño pasando cada subintervalo como argumento a cada programa
esclavo. Por ejemplo, si al maestro le proporcionamos el intervalo entre 1000 y 2000, entonces un
esclavo debe calcular y devolver los números primos comprendidos en el subintervalo entre 1000 y
1500, y el otro esclavo entre 1501 y 2000. El maestro creará dos cauces sin nombre y se encargará
de su redirección para comunicarse con los procesos esclavos. El maestro irá recibiendo y mostrando
en pantalla (también uno a uno) los números primos calculados por los esclavos en orden creciente.
*/

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

/*El sentido de comunicación será de hijo a padre, el hijo escribe y el padre lee (hijo -> padre)*/
int main(int argc, char *argv[]){
    
    //intervalo 0 (i0) para los argumentos; intervalo 1 (i1) para el hijo 1, intervalo 2 (i2) para el hijo 2
    int i0[2], i1[2], i2[2];
    //procesos hijo 1 y hijo 2
    pid_t h1, h2;
    //descriptores de fichero del cauce para el hijo 1 y otro para el hijo 2
    int c1[2], c2[2];
    //mensaje que el padre irá leyendo del hijo
    int mensaje = 0;
    char primo[4], arg1[4], arg2[4];
    
    //comprobamos que se ha pasado como argumento el intervalo
    if(argc < 3){
        perror("ERROR en argumentos. \n El formato es ./maestro limite_inferior limite_superior.\n");
        exit(-1);
    }
    //obtenemos el intervalo
    i0[0] = atoi(argv[1]);
    i0[1] = atoi(argv[2]);
    //comprobamos si han insertado primero el límite sup y después el límite inf
    if(i0[0] > i0[1]){
        i0[0] = i0[1];
        i0[1] = atoi(argv[1]);
    }
    //hacemos la división de intervalos el intervalo1 será para el hijo1
    i1[0] = i0[0];
    i1[1] = ((i0[0] + i0[1])/2);

    //el intervalo2 será para el hijo2
    i2[0] = i1[1] + 1;
    i2[1] = i0[1];

    /* CAUCE 1, PRIMER HIJO */
    //creamos el cauce1
    pipe(c1);

    //creamos el primer hijo y comprobamos que no ha habido error
    if( (h1 = fork()) < 0){
        perror("ERROR en la creación del hijo 1.\n");
        exit(-2);
    }

    //si es el hijo, ejecuta el esclavo
    else if(h1 == 0){
        //redirigimos la salida estándar al descriptor del cauce
        dup2(c1[1], STDOUT_FILENO);
        //pasamos los límites a cadena para pasarlos como argumento a exec
        sprintf(arg1, "%i", i1[0]);
        sprintf(arg2, "%i", i1[1]);
        //ejecutamos el esclavo y comprobamos que no ha habido error
        if( execl("./esclavo", "./esclavo", arg1, arg2, (char*) 0) < 0 ){
            perror("ERROR al ejecutar esclavo1\n");
            exit(-3);
        }
    }

    //si es el padre
    else{
        //como el padre lee, cerramos el descriptor de escritura del cauce
        close(c1[1]);
        //redirigimos la entrada estándar al descriptor del cauce
        dup2(c1[0], STDIN_FILENO);
        printf("Hijo 1: \n");
        //leemos de la entrada estándar el primo que calcula el hijo
        while( mensaje = read(c1[0], primo, sizeof(int)) > 0){
            printf("\t%s es primo\n", primo);
        }
        //cuando el padre termina de leer, cerramos el descriptor de lectura
        //para que el cauce se cierre
        close(c1[0]);
    }

    /* CAUCE 2, SEGUNDO HIJO */
    pipe(c2);
    //creamos el primer hijo y comprobamos que no ha habido error
    if( (h2 = fork()) < 0){
        perror("ERROR en la creación del hijo 2.\n");
        exit(-2);
    }

    //si es el hijo, ejecuta el esclavo
    else if(h2 == 0){
        //como el hijo escribe, cerramos el descriptor de lectura del cauce
        close(c2[0]);
        //redirigimos la salida estándar al descriptor del cauce
        dup2(c2[1], STDOUT_FILENO);
        //pasamos los límites a cadena para pasarlos como argumento a exec
        sprintf(arg1, "%i", i2[0]);
        sprintf(arg2, "%i", i2[1]);
        //ejecutamos el esclavo y comprobamos que no ha habido error
        if( execl("./esclavo", "./esclavo", arg1, arg2, (char*) 0) < 0 ){
            perror("ERROR al ejecutar esclavo1\n");
            exit(-3);
        }
    }

    //si es el padre
    else{
        //como el padre lee, cerramos el descriptor de escritura del cauce
        close(c2[1]);
        //redirigimos la entrada estándar al descriptor del cauce
        dup2(c2[0], STDIN_FILENO);
        printf("Hijo 2: \n");
        //leemos de la entrada estándar el primo que calcula el hijo
        while( mensaje = read(c2[0], primo, sizeof(int)) > 0){
            printf("\t%s es primo\n", primo);
        }
        //cuando el padre termina de leer, cerramos el descriptor de lectura
        //para que el cauce se cierre
        close(c2[0]);
    }
    return 0;
}

