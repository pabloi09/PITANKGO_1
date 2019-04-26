
#ifndef _PITANKGOLIB_H_
#define _PITANKGOLIB_H_

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

#include "fsm.h"
#include "tmr.h"

#define CLK_MS 10 // PERIODO DE ACTUALIZACION DE LA MAQUINA ESTADOS EN MILISEGUNDOS

// INTERVALO DE GUARDA ANTI-REBOTES
#define	DEBOUNCE_TIME	200
#define COL_REFRESH_TIME 45
// DURACION DISPARO IR
#define SHOOTING_PERIOD 1000		// <-----------------------------------------------------------------------------------------------------

// CLAVES PARA MUTEX
#define	SYSTEM_FLAGS_KEY	1
#define	PLAYER_FLAGS_KEY	2
#define	STD_IO_BUFFER_KEY	3
#define FLAG_KEY 			4
#define GAME_FLAGS_KEY 		5

// Distribucion de pines GPIO empleada para el teclado
#define	TECLADO_ROW_1	16
#define	TECLADO_ROW_2	18
#define	TECLADO_ROW_3	22
#define	TECLADO_ROW_4	24

#define	TECLADO_COL_1	11
#define	TECLADO_COL_2	13
#define	TECLADO_COL_3	15
#define	TECLADO_COL_4	19


// Distribucion de pines GPIO empleada para el enlace IR
// ATENCION: Valores asociados a los diodos
#define	IR_TX_PIN		29			// <-----------------------------------------------------------------------------------------------------
#define	IR_RX_PIN		31			// <-----------------------------------------------------------------------------------------------------

// Distribucion de pines GPIO empleada para la reproduccion de efectos
// ATENCION: Valor asociado al pin de salida del reproductor de sonidos
#define PLAYER_PWM_PIN 	33

// Distribucion de pines GPIO empleada para el control de los servos
// ATENCION: Valores asociados a los servo
#define	SERVO_VERTICAL_PIN		32			// <-----------------------------------------------------------------------------------------------------
#define	SERVO_HORIZONTAL_PIN	12			// <-----------------------------------------------------------------------------------------------------

// FLAGS FSM CONTROL DE TECLADO
// ATENCION: Valores a usar por flags_teclado
#define FLAG_KEY_PRESSED 0x01
#define FLAG_TMR_TIMEOUT 0x02

// FLAGS FSM CONTROL DE JUEGO Y TORRETA
// ATENCION: Valores a usar por flags_juego
#define FLAG_SYSTEM_START 		0x01
#define FLAG_JOYSTICK_UP 		0x02
#define FLAG_JOYSTICK_DOWN 		0x04
#define FLAG_JOYSTICK_LEFT		0x08
#define FLAG_JOYSTICK_RIGHT		0x10
#define FLAG_TRIGGER_BUTTON		0x20
#define FLAG_SHOOT_TIMEOUT		0x40
#define FLAG_TARGET_DONE		0x80
#define FLAG_SYSTEM_END			0x100

// FLAGS FSM REPRODUCCION DE EFECTOS DE SONIDO
// ATENCION: valores a usar por flags_player
#define FLAG_START_DISPARO 		1
#define FLAG_START_IMPACTO 		2
#define FLAG_PLAYER_STOP 		4
#define FLAG_PLAYER_END			8
#define FLAG_NOTA_TIMEOUT		16


enum interruption_sources {
	TARGET_ISR,
	TECLADO_FILA_1,
	TECLADO_FILA_2,
	TECLADO_FILA_3,
	TECLADO_FILA_4
};

extern volatile int flags_system;
extern volatile int flags_player;
extern volatile int flags_juego;
extern volatile int flags_teclado;
extern int marcador;
extern int frecuenciasDisparo[];
extern int tiemposDisparo[];
extern int frecuenciasImpacto[];
extern int tiemposImpacto[];
#endif /* _PITANKGOLIB_H_ */
