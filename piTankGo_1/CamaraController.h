/** File Name          : CamaraController.h
  * Description        : Cabecera del controlador de la camara
  */
#ifndef CAMARACONTROLLER_H_
#define CAMARACONTROLLER_H_

/* Includes ------------------------------------------------------------------*/
#include "piTankGoLib.h"
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* Defines -------------------------------------------------------------------*/
#define START "lxterminal -e \"./camara.sh\" "	//Comando de inicio del programa
#define END "./end"			//Comando de fin del programa

/* Private function prototypes -----------------------------------------------*/
void *start();	
void *end();    
void *getIP();  //Funcion que muestra por pantalla la IP de la raspberry

#endif /* CAMARACONTROLLER_H_ */
