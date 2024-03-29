/*
 * piTankGo_1.h
 *
 *  Created on: 11 de enero de 2019
 *      Author: Pablo y Diego
 */

#ifndef _PITANKGO_1_H_
#define _PITANKGO_1_H_

//#include "kbhit.h" // Para poder detectar teclas pulsadas sin bloqueo y leer las teclas pulsadas
#include "piTankGoLib.h"
#include "player.h"
#include "torreta.h"
#include"teclado.h"




// Posibles estados de las FSMs
enum fsm_state {
	WAIT_END_MELODIA,
	MELODIA_INICIO,
	WAIT_START,
	WAIT_KEY,
	WAIT_PUSH,
	WAIT_NEXT,
	WAIT_MOVE,
	WAIT_END,
	JOYSTICK_UP,
	JOYSTICK_DOWN,
	JOYSTICK_LEFT,
	JOYSTICK_RIGHT,
	TRIGGER_BUTTON
};

typedef struct {
	TipoTorreta torreta; // Objeto para el control de la torreta
	TipoPlayer player; // Reproductor de efectos
	char teclaPulsada;  // Variable que almacena la ultima tecla pulsada
	int debug; // Variable que habilita o deshabilita la impresion de mensajes por salida estandar
} TipoSistema;

//------------------------------------------------------
// FUNCIONES DE CONFIGURACION/INICIALIZACION
//------------------------------------------------------
void ConfiguraSistema (TipoSistema *p_sistema);
void InicializaSistema (TipoSistema *p_sistema);
void ConfiguraPins();
//------------------------------------------------------
// FUNCIONES LIGADAS A THREADS ADICIONALES
//------------------------------------------------------
PI_THREAD(thread_explora_teclado_PC);


#endif /* _PITANKGO_1_H_ */
