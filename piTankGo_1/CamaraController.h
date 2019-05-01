#ifndef CAMARACONTROLLER_H_
#define CAMARACONTROLLER_H_

#include "piTankGoLib.h"
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define START "lxterminal -e \"./camara.sh\" "	//Comando de inicio del programa
#define END "./end"			//Comando de fin del programa

pid_t pid;
void *start();	//Funcion que inicia la difusi�n de video
void *end();    //Funcion que acaba la difusi�n de video
void *getIP();  //Funcion que muestra por pantalla la IP de la raspberry
#endif /* CAMARACONTROLLER_H_ */
