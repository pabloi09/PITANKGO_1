#ifndef CAMARACONTROLLER_H_
#define CAMARACONTROLLER_H_

#include "piTankGoLib.h"
#include <string.h>
#include <stdint.h>
#include <sys/types.h> 
#include <unistd.h>  
#include <stdlib.h> 
#include <errno.h>   
#include <sys/wait.h>

#define START "./camara.sh"	//Comando de inicio del programa
#define END "^C"			//Comando de fin del programa

pid_t pid;
void *start();	//Funcion que inicia la comunicacion serie
void *end();
#endif /* CAMARACONTROLLER_H_ */