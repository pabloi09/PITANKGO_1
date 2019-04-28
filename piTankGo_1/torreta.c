
#include "torreta.h"
#include <softPWM.h>

int marcador=0;		//	Variable de control marcador de puntos Arduino

//------------------------------------------------------
// PROCEDIMIENTOS DE INICIALIZACION DE LOS OBJETOS ESPECIFICOS
//------------------------------------------------------

void InicializaTorreta (TipoTorreta *p_torreta) {
	pinMode (IR_TX_PIN, OUTPUT);
	pinMode (IR_RX_PIN, INPUT);
	wiringPiISR (IR_RX_PIN, INT_EDGE_RISING, impacto_isr);

	pinMode (SERVO_HORIZONTAL_PIN, OUTPUT);
	pinMode (SERVO_VERTICAL_PIN, OUTPUT);
	softPwmCreate (SERVO_HORIZONTAL_PIN, p_torreta->servo_x.inicio, SERVO_PWM_RANGE);
	softPwmCreate (SERVO_VERTICAL_PIN, p_torreta->servo_y.inicio, SERVO_PWM_RANGE);
	softPwmWrite(SERVO_HORIZONTAL_PIN, p_torreta->posicion.x);
	softPwmWrite(SERVO_VERTICAL_PIN, p_torreta->posicion.y);

}

//------------------------------------------------------
// FUNCIONES DE ENTRADA O DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

int CompruebaComienzo (fsm_t* this) {
	int result = 0;
	piLock(GAME_FLAGS_KEY);
	result = (flags_juego & FLAG_SYSTEM_START);
	piUnlock (GAME_FLAGS_KEY);
	return result;
}

int CompruebaJoystickUp (fsm_t* this) {
	int result = 0;
	piLock(GAME_FLAGS_KEY);
	result = (flags_juego & FLAG_JOYSTICK_UP);
	piUnlock (GAME_FLAGS_KEY);
	return result;
}

int CompruebaJoystickDown (fsm_t* fsm_player){
	int result = 0;
	piLock(GAME_FLAGS_KEY);
	result = (flags_juego & FLAG_JOYSTICK_DOWN);
	piUnlock (GAME_FLAGS_KEY);
	return result;
}

int CompruebaJoystickLeft (fsm_t* this) {
	int result = 0;
	piLock(GAME_FLAGS_KEY);
	result = (flags_juego & FLAG_JOYSTICK_LEFT);
	piUnlock (GAME_FLAGS_KEY);
	return result;
}

int CompruebaJoystickRight (fsm_t* this) {
	int result = 0;
	piLock(GAME_FLAGS_KEY);
	result = (flags_juego & FLAG_JOYSTICK_RIGHT);
	piUnlock (GAME_FLAGS_KEY);
	return result;
}

int CompruebaTimeoutDisparo (fsm_t* this) {
	int result = 0;
	piLock(GAME_FLAGS_KEY);
	result = (flags_juego & FLAG_SHOOT_TIMEOUT);
	piUnlock (GAME_FLAGS_KEY);
	return result;
}

int CompruebaImpacto (fsm_t* this) {
	int result = 0;
	piLock(GAME_FLAGS_KEY);
	result = (flags_juego & FLAG_TARGET_DONE);
	piUnlock (GAME_FLAGS_KEY);
	return result;
}

int CompruebaTriggerButton (fsm_t* this) {
	int result = 0;
	piLock(GAME_FLAGS_KEY);
	result = (flags_juego & FLAG_TRIGGER_BUTTON);
	piUnlock (GAME_FLAGS_KEY);
	return result;
}

int CompruebaFinalJuego (fsm_t* this) {
	int result = 0;
	piLock(GAME_FLAGS_KEY);
	result = (flags_juego & FLAG_SYSTEM_END);
	piUnlock (GAME_FLAGS_KEY);
	return result;
}

//------------------------------------------------------
// FUNCIONES DE SALIDA O DE ACCION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

void ComienzaSistema (fsm_t* this) {
	TipoTorreta * p_torreta=this->user_data;

	//Configuracion parametros servo horizontal
	p_torreta->servo_x.incremento = SERVO_INCREMENTO;
	p_torreta->servo_x.minimo 	= SERVO_MINIMO;
	p_torreta->servo_x.maximo 	= SERVO_MAXIMO;

	//Configuracion parametros servo vertical
	p_torreta->servo_y.incremento = SERVO_INCREMENTO;
	p_torreta->servo_y.minimo 	= SERVO_MINIMO;
	p_torreta->servo_y.maximo 	= SERVO_MAXIMO;

	p_torreta->servo_x.inicio 	= SERVO_MINIMO + (SERVO_MAXIMO - SERVO_MINIMO)/2;
	p_torreta->servo_y.inicio 	= SERVO_MINIMO + (SERVO_MAXIMO - SERVO_MINIMO)/2;
	p_torreta->posicion.x 	= p_torreta->servo_x.inicio;
	p_torreta->posicion.y 	= p_torreta->servo_y.inicio;

	if(p_torreta->posicion.x > p_torreta->servo_x.maximo)
		p_torreta->posicion.x = p_torreta->servo_x.maximo;
	if(p_torreta->posicion.y > p_torreta->servo_y.maximo)
		p_torreta->posicion.y = p_torreta->servo_y.maximo;

	if(p_torreta->posicion.x < p_torreta->servo_x.minimo)
		p_torreta->posicion.x = p_torreta->servo_x.minimo;
	if(p_torreta->posicion.y < p_torreta->servo_y.minimo)
		p_torreta->posicion.y = p_torreta->servo_y.minimo;

	p_torreta->tmr_shoot=tmr_new(timer_duracion_disparo_isr);	// creamos el temporizador asociado al timeout del disparo
	InicializaTorreta(p_torreta);
	system("./camara.sh");

	//Creamos y lanzamos la hebra de comunicaci�n Arduino
	pthread_t thd1;
	pthread_t thd2;

	if(pthread_create(&thd1,NULL,&joystick,0)!=0){printf("\nNo se pudo iniciar la rutina SerialReader\n");}
	if(pthread_create(&thd2,NULL,&start,0)!=0){printf("\nNo se pudo iniciar la rutina Camara\n");}

}

void MueveTorretaArriba (fsm_t* this) {
	TipoTorreta *p_torreta;
	p_torreta = (TipoTorreta*)(this->user_data);

	piLock(GAME_FLAGS_KEY);
	flags_juego &= (~FLAG_JOYSTICK_UP);
	piUnlock (GAME_FLAGS_KEY);

	if(p_torreta->posicion.y + p_torreta->servo_y.incremento <= p_torreta->servo_y.maximo) {
		p_torreta->posicion.y = p_torreta->posicion.y + p_torreta->servo_y.incremento;

		softPwmWrite(SERVO_VERTICAL_PIN, p_torreta->posicion.y);

		printf("[SERVO_Y][POSICION]=[%d]\n", p_torreta->posicion.y);
		fflush(stdout);
	}
}

void MueveTorretaAbajo (fsm_t* this) {
	TipoTorreta *p_torreta;
	p_torreta = (TipoTorreta*)(this->user_data);

	piLock(GAME_FLAGS_KEY);
	flags_juego &= (~FLAG_JOYSTICK_DOWN);
	piUnlock (GAME_FLAGS_KEY);

	if(p_torreta->posicion.y - p_torreta->servo_y.incremento >= p_torreta->servo_y.minimo) {
		p_torreta->posicion.y = p_torreta->posicion.y - p_torreta->servo_y.incremento;

		softPwmWrite(SERVO_VERTICAL_PIN, p_torreta->posicion.y);

		printf("[SERVO_Y][POSICION]=[%d]\n", p_torreta->posicion.y);
		fflush(stdout);
	}
}

void MueveTorretaIzquierda (fsm_t* this) {
	TipoTorreta *p_torreta;
	p_torreta = (TipoTorreta*)(this->user_data);

	piLock(GAME_FLAGS_KEY);
	flags_juego &= (~FLAG_JOYSTICK_LEFT);
	piUnlock (GAME_FLAGS_KEY);

	if(p_torreta->posicion.x - p_torreta->servo_x.incremento >= p_torreta->servo_x.minimo) {
		p_torreta->posicion.x = p_torreta->posicion.x - p_torreta->servo_x.incremento;

		softPwmWrite(SERVO_HORIZONTAL_PIN, p_torreta->posicion.x);

		printf("[SERVO_X][POSICION]=[%d]\n", p_torreta->posicion.x);
		fflush(stdout);
	}
}

void MueveTorretaDerecha (fsm_t* this) {
	TipoTorreta *p_torreta;
	p_torreta = (TipoTorreta*)(this->user_data);

	piLock(GAME_FLAGS_KEY);
	flags_juego &= (~FLAG_JOYSTICK_RIGHT);
	piUnlock (GAME_FLAGS_KEY);

	if(p_torreta->posicion.x + p_torreta->servo_x.incremento <= p_torreta->servo_x.maximo) {
		p_torreta->posicion.x = p_torreta->posicion.x + p_torreta->servo_x.incremento;

		softPwmWrite(SERVO_HORIZONTAL_PIN, p_torreta->posicion.x);

		printf("[SERVO_X][POSICION]=[%d]\n", p_torreta->posicion.x);
		fflush(stdout);
	}
}

void DisparoIR (fsm_t* this) {
	TipoTorreta *p_torreta;
	p_torreta = (TipoTorreta*)(this->user_data);

	piLock(GAME_FLAGS_KEY);
	flags_juego &= (~FLAG_TRIGGER_BUTTON);
	piUnlock (GAME_FLAGS_KEY);

	piLock (PLAYER_FLAGS_KEY);
	flags_player |= FLAG_START_DISPARO;
	piUnlock (PLAYER_FLAGS_KEY);

	digitalWrite (IR_TX_PIN, HIGH);
	tmr_startms(p_torreta->tmr_shoot,SHOOTING_PERIOD);
}

void FinalDisparoIR (fsm_t* this) {

	piLock(GAME_FLAGS_KEY);
	flags_juego &= (~FLAG_SHOOT_TIMEOUT);
	piUnlock (GAME_FLAGS_KEY);

	digitalWrite (IR_TX_PIN, LOW);

}

void ImpactoDetectado (fsm_t* this) {

	piLock(GAME_FLAGS_KEY);
	flags_juego &= (~FLAG_TARGET_DONE);
	piUnlock (GAME_FLAGS_KEY);

	piLock (PLAYER_FLAGS_KEY);
	flags_player |= FLAG_START_IMPACTO;
	piUnlock (PLAYER_FLAGS_KEY);

	marcador=1;		//marcador=1 comunica al Arduino que sume un punto al marcador
}

void FinalizaJuego (fsm_t* this) {
	TipoTorreta *p_torreta;
	p_torreta = (TipoTorreta*)(this->user_data);

	marcador=2;		//marcador=2 termina la comunicaci�n serie

	piLock(GAME_FLAGS_KEY);
	flags_juego &= (~FLAG_SYSTEM_START);
	flags_juego &= (~FLAG_SYSTEM_END);
	piUnlock (GAME_FLAGS_KEY);

	softPwmWrite(SERVO_VERTICAL_PIN, p_torreta->servo_y.minimo);		//Dejamos la torreta en posici�n de descanso
	softPwmWrite(SERVO_HORIZONTAL_PIN, p_torreta->servo_x.inicio);
	delay (40);
	softPwmStop(SERVO_VERTICAL_PIN);
	softPwmStop(SERVO_HORIZONTAL_PIN);

	piLock(PLAYER_FLAGS_KEY);
	flags_system=1;					//Detiene la ejecucion del bucle en [PiTankGo_1.c/main()]
	piUnlock(PLAYER_FLAGS_KEY);
	end();
}

//------------------------------------------------------
// FUNCIONES DE INTERRUPCION
//------------------------------------------------------

void impacto_isr (void) {
	piLock(GAME_FLAGS_KEY);
	flags_juego |= FLAG_TARGET_DONE;
	piUnlock (GAME_FLAGS_KEY);
}

void timer_duracion_disparo_isr (union sigval value) {
	piLock(GAME_FLAGS_KEY);
	flags_juego |= FLAG_SHOOT_TIMEOUT;
	piUnlock (GAME_FLAGS_KEY);
	printf("\nHas fallado el disparo\n");
	fflush(stdout);
}
