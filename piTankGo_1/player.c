
#include "player.h"

//------------------------------------------------------
// PROCEDIMIENTOS DE INICIALIZACION DE LOS OBJETOS ESPECIFICOS
//------------------------------------------------------

//------------------------------------------------------
// void InicializaEfecto (TipoEfecto *p_efecto)
//
// Descripcion: inicializa los parametros del objeto efecto.
// Ejemplo de uso:
//
// ...
//
// TipoEfecto efecto_demo;
//
// if ( InicializaEfecto (&efecto_demo, "DISPARO", frecuenciasDisparo, tiemposDisparo, 16) < 0 ) {
// 		printf("\n[ERROR!!!][InicializaEfecto]\n");
// 		fflush(stdout);
// 		}
//
// ...
//
//------------------------------------------------------
int InicializaEfecto (TipoEfecto *p_efecto, char *nombre, int *array_frecuencias, int *array_duraciones, int num_notas) {
	for (int i = 0; i < num_notas; i++) {
	p_efecto->duraciones[i]=array_duraciones[i];
	p_efecto->frecuencias[i]=array_frecuencias[i];
   }
    strncpy(nombre, p_efecto->nombre, sizeof(nombre));
	p_efecto->num_notas=num_notas;
	return p_efecto->num_notas;
}

// Procedimiento de inicializacion del objeto especifico
// Nota: parte inicialización común a InicializaPlayDisparo y InicializaPlayImpacto
void InicializaPlayer (TipoPlayer *p_player) {
	TipoEfecto *efecto=p_player->p_efecto;
	p_player->duracion_nota_actual=efecto->duraciones[0];
	p_player->frecuencia_nota_actual=efecto->frecuencias[0];
	p_player->posicion_nota_actual=0;
}

//------------------------------------------------------
// FUNCIONES DE ENTRADA O DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------
int CompruebaStartDisparo (fsm_t* this) {
	int result = 0;
	piLock (SYSTEM_FLAGS_KEY);
	result=flags_system & FLAG_START_DISPARO;
	flags_player=0;
	piUnlock (SYSTEM_FLAGS_KEY);
	return result;
}

int CompruebaStartImpacto (fsm_t* this) {
	int result = 0;
	piLock (SYSTEM_FLAGS_KEY);
	result=flags_player & FLAG_START_IMPACTO;
	flags_player=0;
	piUnlock (SYSTEM_FLAGS_KEY);
	return result;
}

int CompruebaNuevaNota (fsm_t* this){
	int result;
	piLock (SYSTEM_FLAGS_KEY);
	result=!(flags_player & FLAG_PLAYER_END);
	flags_player=0;
	piUnlock (SYSTEM_FLAGS_KEY);
	return result;
}

int CompruebaNotaTimeout (fsm_t* this) {
	int result;
	piLock (SYSTEM_FLAGS_KEY);
	result=flags_player & FLAG_NOTA_TIMEOUT;
	flags_player=0;
	piUnlock (SYSTEM_FLAGS_KEY);
	return result;
}

int CompruebaFinalEfecto (fsm_t* this) {
	int result = 0;
	piLock (SYSTEM_FLAGS_KEY);
	result=flags_player & FLAG_PLAYER_END;
	flags_player=0;
	piUnlock (SYSTEM_FLAGS_KEY);
	return result;

}

//------------------------------------------------------
// FUNCIONES DE SALIDA O DE ACCION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

void InicializaPlayDisparo (fsm_t* this) {
	TipoPlayer * p_player=this->user_data;
	p_player->p_efecto=&p_player->efecto_disparo;
	InicializaPlayer(p_player);
	int mil=millis()+p_player->duracion_nota_actual;
	delay_until(mil);
	
}

void InicializaPlayImpacto (fsm_t* this) {
	TipoPlayer * p_player=this->user_data;
	p_player->p_efecto=&p_player->efecto_impacto;
	InicializaPlayer(p_player);
	int mil=millis()+p_player->duracion_nota_actual;
	delay_until(mil);
}

void ComienzaNuevaNota (fsm_t* this) {
	TipoPlayer * p_player=this->user_data;
	int mil=millis()+p_player->duracion_nota_actual;
	delay_until(mil);
}

void ActualizaPlayer (fsm_t* this) {
	TipoPlayer * player=this->user_data;
	TipoEfecto *efecto=player->p_efecto;
	player->posicion_nota_actual+=1;
	player->duracion_nota_actual=efecto->duraciones[player->posicion_nota_actual];
	player->frecuencia_nota_actual=efecto->frecuencias[player->posicion_nota_actual];

}

void FinalEfecto (fsm_t* this) {
	TipoPlayer * player=this->user_data;
	InicializaPlayer(player);
}

//------------------------------------------------------
// PROCEDIMIENTOS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------

static void timer_player_duracion_nota_actual_isr (union sigval value) {
	// A completar por el alumno...
	// ...
}
