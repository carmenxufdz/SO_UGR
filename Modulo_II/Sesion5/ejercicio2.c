/*
ejercicio2.c
Escribe un programa en C llamado contador, tal que cada vez que reciba una señal que se pueda
manejar, muestre por pantalla la señal y el número de veces que se ha recibido ese tipo de señal, y un
mensaje inicial indicando las señales que no puede manejar. En el cuadro siguiente se muestra un ejemplo de
ejecución del programa.
*/

#include <sys/types.h>
#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#define SIG 31

//vector de señales
static int contador_sig[SIG];

/*función para contar las señales que se recibe - s es el número de la señal*/
void contador(int s){
    //incrementamos el número de veces que se ha recibido dicha señal
    contador_sig[s-1]++;
    //imprimimos por pantalla el número de veces que se ha recibido la señal
    printf("La señal %i se ha recibido %i veces\n", s, contador_sig[s-1]);
}

int main(int argc, char * argv[]){
    //estructura para trabajar con la llamada sigaction
    struct sigaction cont;
    
    //para que envíe los mensajes conforme los reciba y no en bloques
    if(setvbuf(stdout, NULL, _IONBF, 0)){
        perror("\nError en setvbuf");
    }
    
    /*Inicializamos los campos de cont (sa_handler, sa_mask y sa_flags*/
    //especificamos que la función manejadora de señales es contador
    cont.sa_handler = contador;
    
    //inicializamos a vacío el conjunto de señales
    sigemptyset(&cont.sa_mask);
    
    //no modificamos el comportamiento del proceso
    cont.sa_flags = 0;
    
    /*inicilizamos el vector de señales a 0*/
    for(int i = 0; i < SIG; i++){
        contador_sig[i] = 0;
    }

    /*establecemos el manejador contador para cada señal que se reciba con sigaction*/
    for(int i = 1; i <= SIG; i++){
        //si la señal es SIGKILL o SIGSTOP, no le asignamos manejador
        if(i != SIGKILL && i != SIGSTOP){
            //asignamos el manejador cont a la señal i
            //si hay error, se imprime por pantalla
            if( sigaction(i, &cont, NULL) < 0){
                perror("ERROR al intentar establecer manejador de señal\n");
                exit(-2);
            }
        }
    }
    
    //esperamos indefinidamente a recibir las señales
    for(;;){}

    return 0;
}

/*
Ejecutamos el programa en background
$ ./contador &
[1] 77145
Le vamos enviando señales y vemos cómo las contabiliza
$ kill -2 77145
La señal 2 se ha recibido 1 veces
$ kill -3 77145
La señal 3 se ha recibido 1 veces
$ kill -4 77145
La señal 4 se ha recibido 1 veces
$ kill -5 77145
La señal 5 se ha recibido 1 veces
$ kill -5 77145
La señal 5 se ha recibido 2 veces
$ kill -5 77145
La señal 5 se ha recibido 3 veces
$ kill -18 77145
La señal 18 se ha recibido 1 veces
$ kill -18 77145
La señal 18 se ha recibido 2 veces
$ kill -18 77145
La señal 18 se ha recibido 3 veces
$ kill -9 77145
//con esta señal finalizamos el proceso
[1]+ Terminado (killed)
./contador
*/