/*
ejercicio3.c
Escribe un programa que suspenda la ejecución del proceso actual hasta que se reciba la señal
SIGUSR1. Consulta en el manual en línea sigemptyset para conocer las distintas operaciones que permiten
configurar el conjunto de señales de un proceso.
*/
#include <stdio.h>
#include <signal.h>

void manejador(int i){
    printf("Acaba de desbloquearse el proceso.\n");
}

int main(){
    sigset_t mascara;
    struct sigaction tarea;
    
    //inicializamos la máscara
    sigemptyset(&tarea.sa_mask);
    
    //le asignamos manejador
    tarea.sa_handler = manejador;
    
    //le asignamos manejador a SIGUSR1
    sigaction(SIGUSR1, &tarea, NULL);
    tarea.sa_flags = 0;
    
    //vacíamos y añadimos todas las señales a máscara
    sigemptyset(&mascara);
    sigfillset(&mascara);
    
    //eliminamos SIGUSR1 de la máscara
    sigdelset(&mascara, SIGUSR1);
    
    //suspende el proceso hasta que se envía alguna de las señales que NO están en la máscara
    sigsuspend(&mascara);
    
    return 0;
}