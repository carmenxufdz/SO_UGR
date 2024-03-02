/*
esclavo.c 
El programa esclavo tiene como argumentos el extremo inferior y superior del intervalo sobre el que
buscará números primos. Para identificar un número primo utiliza el siguiente método concreto: un
número n es primo si no es divisible por ningún k tal que 2 < k <= sqrt(n), donde sqrt corresponde a
la función de cálculo de la raíz cuadrada (consulte dicha función en el manual). El esclavo envía al
maestro cada primo encontrado como un dato entero (4 bytes) que escribe en la salida estándar, la
cuál se tiene que encontrar redireccionada a un cauce sin nombre. Los dos cauces sin nombre
necesarios, cada uno para comunicar cada esclavo con el maestro, los creará el maestro inicialmente.
Una vez que un esclavo haya calculado y enviado (uno a uno) al maestro todos los primos en su
correspondiente intervalo terminará.

Hay que ejecutar:
    gcc esclavo.c -lm -o esclavo
si no, sale el siguiente error:
    esclavo.c:(.text+0x1c): referencia a `sqrt' sin definir 
    collect2: error: ld returned 1 exit status
*/

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>

//función que calcula si n es un número primo; devuelve: 0 si no es primo; 1 si es primo
int esPrimo(int n){
    int primo = 1;
    //recorremos desde 2 < k <= sqrt(n)
    for(int i = 2; i <= sqrt(n) && primo; i++){
    //si es divisible, ya no es primo
        if(n%i == 0){
        primo = 0;
        }
    }
    return primo;
}

int main(int argc, char *argv[]){
    int lim_inf, lim_sup;
    char primo[4];
    
    //comprobamos que se ha pasado como argumento el intervalo
    if(argc < 3){
        perror("ERROR en argumentos. \nEl formato es ./esclavo limite_inferior limite_superior.\n");
        exit(-1);
    }
    //obtenemos el intervalo
    lim_inf = atoi(argv[1]);
    lim_sup = atoi(argv[2]);
    
    //comprobamos si han insertado primero el lim sup y después el lim inf
    if(lim_inf > lim_sup){
        lim_inf = lim_sup;
        lim_sup = atoi(argv[1]);
    }
    //calculamos los primos en dicho intervalo
    for(int i = lim_inf; i <= lim_sup; i++){
        //vemos si es primo
        if(esPrimo(i)){
            sprintf(primo, "%i", i);
            //si es primo, lo escribimos por la salida estándar para que lo lea el padre
            write(STDOUT_FILENO, primo, sizeof(int));
        }
    }
    return 0;
}