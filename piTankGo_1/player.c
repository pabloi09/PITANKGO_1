
#include "player.h"


//------------------------------------------------------
// PROCEDIMIENTOS DE INICIALIZACION DE LOS OBJETOS ESPECIFICOS
//------------------------------------------------------

int InicializaEfecto (TipoEfecto *p_efecto, char *nombre, int *array_frecuencias, int *array_duraciones, int num_notas) {
	int i = 0;
	for (i = 0; i < num_notas; i++) {
	p_efecto->duraciones[i]=array_duraciones[i];
	p_efecto->frecuencias[i]=array_frecuencias[i];
   }
    strcpy(p_efecto->nombre, nombre);
	p_efecto->num_notas=num_notas;
	return p_efecto->num_notas;
}

// Procedimiento de inicializacion del objeto especifico
// Nota: parte inicializacion comun a InicializaPlayDisparo y InicializaPlayImpacto
void InicializaPlayer (TipoPlayer *p_player) {
	p_player->posicion_nota_actual=0;
	p_player->tmr_notas=tmr_new(timer_player_duracion_nota_actual_isr);	// creamos el temporizador asociado al timeout de las notas

}

void IniciaMelodia (fsm_t* this) {
	TipoPlayer * p_player=this->user_data;
	//delay(1000);
	FinalEfecto(this);

	//Inicio melodia de fondo
	p_player->p_efecto=&(p_player->efecto_melodia);
	p_player->duracion_nota_actual=p_player->p_efecto->duraciones[0];
	p_player->frecuencia_nota_actual=p_player->p_efecto->frecuencias[0];

	softToneWrite (PLAYER_PWM_PIN,p_player->frecuencia_nota_actual);
	tmr_startms(p_player->tmr_notas,p_player->duracion_nota_actual);
}

//------------------------------------------------------
// FUNCIONES DE ENTRADA O DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------
int CompruebaStartGame (fsm_t* this) {
	int result = 0;
	piLock (GAME_FLAGS_KEY);
	result=flags_juego & FLAG_SYSTEM_START;
	flags_player &= ~FLAG_START_DISPARO;
	piUnlock (GAME_FLAGS_KEY);
	return result;
}

int CompruebaStartDisparo (fsm_t* this) {
	int result = 0;
	piLock (SYSTEM_FLAGS_KEY);
	result=flags_player & FLAG_START_DISPARO;
	flags_player &= ~FLAG_START_DISPARO;
	piUnlock (SYSTEM_FLAGS_KEY);
	return result;
}

int CompruebaStartImpacto (fsm_t* this) {
	int result = 0;
	piLock (SYSTEM_FLAGS_KEY);
	result=flags_player & FLAG_START_IMPACTO;
	flags_player &= ~FLAG_START_IMPACTO;
	piUnlock (SYSTEM_FLAGS_KEY);
	return result;
}

int CompruebaNuevaNota (fsm_t* this){
	int result=0;
	piLock (SYSTEM_FLAGS_KEY);
	result=!(flags_player & FLAG_PLAYER_END);
	flags_player &= ~FLAG_PLAYER_END;
	piUnlock (SYSTEM_FLAGS_KEY);
	return result;
}

int CompruebaNotaTimeout (fsm_t* this) {
	int result=0;
	piLock (SYSTEM_FLAGS_KEY);
	result=flags_player & FLAG_NOTA_TIMEOUT;
	flags_player &= ~FLAG_NOTA_TIMEOUT;
	piUnlock (SYSTEM_FLAGS_KEY);
	return result;
}

int CompruebaFinalEfecto (fsm_t* this) {
	int result = 0;
	piLock (SYSTEM_FLAGS_KEY);
	result=flags_player & FLAG_PLAYER_END;
	flags_player &= ~FLAG_PLAYER_END;
	piUnlock (SYSTEM_FLAGS_KEY);
	return result;

}

//------------------------------------------------------
// FUNCIONES DE SALIDA O DE ACCION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

void SilenciaMelodia (fsm_t* this) {
	FinalEfecto(this);
}

void InicializaPlayDisparo (fsm_t* this) {
	TipoPlayer * p_player=this->user_data;
	p_player->p_efecto=&(p_player->efecto_disparo);
	p_player->duracion_nota_actual=p_player->p_efecto->duraciones[0];
	p_player->frecuencia_nota_actual=p_player->p_efecto->frecuencias[0];
	p_player->posicion_nota_actual=0;

	softToneWrite (PLAYER_PWM_PIN,p_player->frecuencia_nota_actual);
	tmr_startms(p_player->tmr_notas,p_player->duracion_nota_actual);
}

void InicializaPlayImpacto (fsm_t* this) {
	TipoPlayer * p_player=this->user_data;
	p_player->p_efecto=&(p_player->efecto_impacto);
	p_player->duracion_nota_actual=p_player->p_efecto->duraciones[0];
	p_player->frecuencia_nota_actual=p_player->p_efecto->frecuencias[0];
	p_player->posicion_nota_actual=0;

}

void ComienzaNuevaNota (fsm_t* this) {
	TipoPlayer * p_player=this->user_data;

	softToneWrite (PLAYER_PWM_PIN,p_player->frecuencia_nota_actual);
	tmr_startms(p_player->tmr_notas,p_player->duracion_nota_actual);
}

void ActualizaPlayer (fsm_t* this) {
	TipoPlayer * p_player=(TipoPlayer*)(this->user_data);
	int a=p_player->posicion_nota_actual;
	int b=p_player->p_efecto->num_notas-1;
	if (a<b) {
		p_player->posicion_nota_actual++;
		p_player->duracion_nota_actual=p_player->p_efecto->duraciones[p_player->posicion_nota_actual];
		p_player->frecuencia_nota_actual=p_player->p_efecto->frecuencias[p_player->posicion_nota_actual];

	} else {
		piLock (PLAYER_FLAGS_KEY);
		flags_player |= FLAG_PLAYER_END;	// En el caso de ultima nota activamos el bit de FLAG_PLAYER_END
		piUnlock (PLAYER_FLAGS_KEY);
	}
}

void FinalEfecto (fsm_t* this) {

	TipoPlayer * p_player=this->user_data;
	p_player->posicion_nota_actual=0;
	softToneWrite (PLAYER_PWM_PIN,0);
}

//------------------------------------------------------
// PROCEDIMIENTOS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------

void timer_player_duracion_nota_actual_isr (union sigval value) {
	piLock (PLAYER_FLAGS_KEY);
	flags_player |= FLAG_NOTA_TIMEOUT;
	piUnlock (PLAYER_FLAGS_KEY);
}
