#ifndef CAMARACONTROLLER_H_
#define CAMARACONTROLLER_H_

#include "piTankGoLib.h"
#include <string.h>
#include <stdint.h>

#define START "./camara.sh"	//Comando de inicio del programa
#define END "^C"			//Comando de fin del programa

void *start();	//Funcion que inicia la comunicacion serie
void *end();
#endif /* CAMARACONTROLLER_H_ */