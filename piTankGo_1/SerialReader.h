/*
 * SerialReader.h
 *
 *  Created on: 22 abr. 2019
 *      Author: diego
 */

#ifndef SERIALREADER_H_
#define SERIALREADER_H_

#include <wiringSerial.h>
#include <string.h>
#include <stdint.h>
#include "piTankGoLib.h"

#define USB "/dev/ttyACM0"	//Ruta en la RaspberryPi al dispositivo
#define SPEED 9600			//Velocidad de comunicacion serie en baudios
#define MSGSIZE	5			//Tamaño en Bytes de los mensajes enviados: " dcha", " izqd", " arrb", " abjo"
#define bsync 0x3e			//Byte de sincronismo
#define RIGHT ">dcha"	//comando ASCII
#define LEFT ">izqd" 	//comando ASCII
#define UP ">arrb" 	//comando ASCII
#define DOWN ">abjo" 	//comando ASCII
#define NO_CONNECT	"No se pudo encontrar el mando, asegúrese de que está conectado o use el teclado\n"

void *joystick();	//Funcion que inicia la comunicacion serie

#endif /* SERIALREADER_H_ */
