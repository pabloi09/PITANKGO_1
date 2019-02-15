
#include "player.h"
#include <wiringPi.h>
#include <string.h>
#include <softTone.h>

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
// Nota: parte inicialización común a InicializaPlayDisparo y InicializaPlayImpacto
void InicializaPlayer (TipoPlayer *p_player) {
	p_player->posicion_nota_actual=0;
}

//------------------------------------------------------
// FUNCIONES DE ENTRADA O DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------
int CompruebaStartDisparo (fsm_t* this) {
	int result = 0;
	piLock (SYSTEM_FLAGS_KEY);
	result=flags_player & FLAG_START_DISPARO;	//He cambiado todos los && por
	flags_player &= ~FLAG_START_DISPARO;        //&. & es el operador bit a bit
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
	int result=0;//El fallo tambien podia estar en que aqui no inicializabamos result a 0
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

void InicializaPlayDisparo (fsm_t* this) {
	TipoPlayer * p_player=this->user_data;
	p_player->p_efecto=&(p_player->efecto_disparo);
	p_player->duracion_nota_actual=p_player->p_efecto->duraciones[0];
	p_player->frecuencia_nota_actual=p_player->p_efecto->frecuencias[0];
	piLock(STD_IO_BUFFER_KEY);
	printf ("Frecuencias disparo:\n%d\n",p_player->frecuencia_nota_actual);
	fflush(stdout);
	piUnlock(STD_IO_BUFFER_KEY);
}

void InicializaPlayImpacto (fsm_t* this) {
	TipoPlayer * p_player=this->user_data;
	p_player->p_efecto=&(p_player->efecto_impacto);
	p_player->duracion_nota_actual=p_player->p_efecto->duraciones[0];
	p_player->frecuencia_nota_actual=p_player->p_efecto->frecuencias[0];
	piLock(STD_IO_BUFFER_KEY);
	printf ("Frecuencias impacto:\n%d\n",p_player->frecuencia_nota_actual);
	fflush(stdout);
	piUnlock(STD_IO_BUFFER_KEY);
}

void ComienzaNuevaNota (fsm_t* this) {
	TipoPlayer * p_player=this->user_data;
	piLock(STD_IO_BUFFER_KEY);
	printf ("%d\n",p_player->frecuencia_nota_actual);
	//Esto estaba puesto en el sitio que no era
	softToneWrite (IR_TX_PIN,p_player->frecuencia_nota_actual) ;
	fflush(stdout);
	piUnlock(STD_IO_BUFFER_KEY);
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
	piLock(STD_IO_BUFFER_KEY);
	printf("\nDisparo terminado\n");
	fflush(stdout);
	piUnlock(STD_IO_BUFFER_KEY);
	TipoPlayer * p_player=this->user_data;
	free(p_player->p_efecto);
	free(&p_player->duracion_nota_actual);
	free(&p_player->frecuencia_nota_actual);
	InicializaPlayer(p_player);
}

//------------------------------------------------------
// PROCEDIMIENTOS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------

//static void timer_player_duracion_nota_actual_isr (union sigval value) {
	// A completar por el alumno...
	// ...
//}
