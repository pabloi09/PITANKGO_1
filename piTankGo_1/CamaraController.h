#ifndef CAMARACONTROLLER_H_
#define CAMARACONTROLLER_H_

#include "piTankGoLib.h"
#include <stdio.h>
#include <sys/types.h> 
#include <unistd.h>  
#include <stdlib.h> 
#include <errno.h>   
#include <sys/wait.h>
#include  <signal.h>

#define START "lxterminal -e \"./camara.sh\" & PID=$! "	//Comando de inicio del programa
#define END "./end"			//Comando de fin del programa

pid_t pid;
void *start();	//Funcion que inicia la comunicacion serie
void *end();
#endif /* CAMARACONTROLLER_H_ */
