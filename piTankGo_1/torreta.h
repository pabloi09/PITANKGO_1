/*
 * torreta.h
 *
 *  Created on: 7 de marzo de 2019
 *      Author: Diego
 */

#ifndef _TORRETA_H_
#define _TORRETA_H_

#include <softPwm.h>
#include "piTankGoLib.h"

//Parametros del PWM: 1)num de intervalos de 100us en el ciclo del PWM		2)num de intervalos incrementados
//3)PWM minimo		4)PWM maximo
#define SERVO_PWM_RANGE		400
#define SERVO_Y_INCREMENTO	1
#define SERVO_X_INCREMENTO	2
#define SERVO_MINIMO		9
#define SERVO_MAXIMO		22


typedef struct {
	int inicio; // Valor correspondiente a la posicion inicial del servo
	int incremento; // Cuantía en que se incrementa el valor de la posicion con cada movimiento del servo
	int minimo; // Valor mínimo correspondiente a la posicion del servo
	int maximo; // Valor maximo correspondiente a la posicion del servo
} TipoServo;

typedef struct {
	int x; // Coordenada x correspondiente a la posicion del servo horizontal
	int y; // Coordenada y correspondiente a la posicion del servo vertical
} TipoPosicionTorreta;

typedef struct {
	TipoPosicionTorreta posicion;
	TipoServo servo_x;
	TipoServo servo_y;

	// A completar por el alumno (declaracion del temporizador para control duracion disparo)
	tmr_t* tmr_shoot;
} TipoTorreta;



// Prototipos de procedimientos de inicializacion de los objetos especificos
void InicializaTorreta (TipoTorreta *p_torreta);

// Prototipos de funciones de interrupcion
void impacto_isr (void);

// Prototipos de funciones de entrada
int CompruebaComienzo (fsm_t* this);
int CompruebaJoystickUp (fsm_t* this);
int CompruebaJoystickDown (fsm_t* fsm_player);
int CompruebaJoystickLeft (fsm_t* this);
int CompruebaJoystickRight (fsm_t* this);
int CompruebaTimeoutDisparo (fsm_t* this);
int CompruebaImpacto (fsm_t* this);
int CompruebaTriggerButton (fsm_t* this);
int CompruebaFinalJuego (fsm_t* this);

// Prototipos de funciones de salida
void ComienzaSistema (fsm_t* this);
void MueveTorretaArriba (fsm_t* this);
void MueveTorretaAbajo (fsm_t* this);
void MueveTorretaIzquierda (fsm_t* this);
void MueveTorretaDerecha (fsm_t* this);
void DisparoIR (fsm_t* this);
void FinalDisparoIR (fsm_t* this);
void ImpactoDetectado (fsm_t* this);
void FinalizaJuego (fsm_t* this);

// Prototipos de procedimientos de atencion a las interrupciones
void timer_duracion_disparo_isr (union sigval value);


#endif /* _TORRETA_H_ */
