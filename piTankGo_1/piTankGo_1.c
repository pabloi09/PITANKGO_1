#include "piTankGo_1.h"


int frecuenciasDisparo[16] = {2500,2400,2300,2200,2100,2000,1900,1800,1700,1600,1500,1400,1300,1200,1100,1000};
int tiemposDisparo[16] = {75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75};
int frecuenciasImpacto[32] = {97,109,79,121,80,127,123,75,119,96,71,101,98,113,92,70,114,75,86,103,126,118,128,77,114,119,72};
int tiemposImpacto[32] = {10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10};
int frecuenciasFondo[48] = {860,0,860,1308,0,1173,1285,1451,1285,0,860,0,860,1308,0,1173,1095,972,742,0,860,0,860,1308,0,1173,1285,1451,1285,0,860,0,860,1308,0,1138,1095,860,0,742,0,860,0,590,0,742,860,0,};
int tiemposFondo[48] = {175,50,175,1000,150,150,150,150,1000,300,175,50,175,1000,150,150,150,150,1000,300,175,50,175,1000,150,150,150,150,1000,300,175,50,175,1000,150,200,150,1000,500,150,50,350,75,150,75,150,100,2000};

pthread_t *thread_explora_teclado=NULL;	//Manejador de hebra que explora teclado

volatile int flags_juego = 0;
volatile int flags_system = 0;
volatile int flags_player = 0;
volatile int flags_teclado = 0;
TipoTeclado teclado;


//------------------------------------------------------
// FUNCIONES DE CONFIGURACION/INICIALIZACION
//------------------------------------------------------

// int ConfiguracionSistema (TipoSistema *p_sistema): procedimiento de configuracion del sistema.
// Realizará, entra otras, todas las operaciones necesarias para:
// configurar el uso de posibles librerías (e.g. PiGPIO),
// configurar las interrupciones externas asociadas a los pines GPIO,
// configurar las interrupciones periódicas y sus correspondientes temporizadores,
// crear, si fuese necesario, los threads adicionales que pueda requerir el sistema
void ConfiguraSistema (TipoSistema *p_sistema) {
	piLock(SYSTEM_FLAGS_KEY);
	piLock(STD_IO_BUFFER_KEY);

	// configura wiringPi
	if (wiringPiSetupPhys () < 0) {
		printf ("No se pudo configurar wiringPi\n");
	}
	ConfiguraPins(p_sistema);
	piUnlock(STD_IO_BUFFER_KEY);
	piUnlock(SYSTEM_FLAGS_KEY);
}
void ConfiguraPins(TipoSistema *p_sistema){
	//Configura salida sonidos
	pinMode (PLAYER_PWM_PIN, OUTPUT);
	softToneCreate(PLAYER_PWM_PIN);
	//Configuramos teclado
	if(IniciaInOutTeclas(&teclado)<0){
		printf ("No se pudo configurar el teclado\n");
	};

}



// int InicializaSistema (TipoSistema *p_sistema): procedimiento de inicializacion del sistema.
// Realizará, entra otras, todas las operaciones necesarias para:
// la inicializacion de los diferentes elementos de los que consta nuestro sistema,
// la torreta, los efectos, etc.
// igualmente arrancará el thread de exploración del teclado del PC
void InicializaSistema (TipoSistema *p_sistema) {
	piLock(SYSTEM_FLAGS_KEY);

	char * nombre_disparo="disparo";
	char * nombre_impacto="impacto";

	TipoPlayer *player=&(p_sistema->player);
	printf("\nBIENVENIDO A SU TORRETA SOLDADO\n");
	printf("*: Iniciar juego\tA: Disparar\tD: Finalizar juego\n2: Up\t4: Left\t6: Right\t8: Down\n El resto de teclas saldran por pantalla\n");
			fflush(stdout);
	if(InicializaEfecto(&(player->efecto_disparo),nombre_disparo,frecuenciasDisparo,tiemposDisparo,16)<1){
		printf("\n[ERROR!!!][InicializaEfecto]\n");
		fflush(stdout);
	}
	if(InicializaEfecto(&(player->efecto_impacto),nombre_impacto,frecuenciasImpacto,tiemposImpacto,32)<1){
		printf("\n[ERROR!!!][InicializaEfecto]\n");
		fflush(stdout);
	}
	if(InicializaEfecto(&(player->efecto_melodia),"melodia",frecuenciasFondo,tiemposFondo,48)<1){
		printf("\n[ERROR!!!][InicializaMelodia]\n");
		fflush(stdout);
	}

	InicializaPlayer(player);
	InicializaTorreta(&p_sistema->torreta);

	p_sistema->debug=0;	// Modo traza desactivado
	p_sistema->teclaPulsada = '\0';

	piUnlock (STD_IO_BUFFER_KEY);
	piUnlock(SYSTEM_FLAGS_KEY);

}


// wait until next_activation (absolute time)
void delay_until (unsigned int next) {
	unsigned int now = millis();
	if (next > now) {
		delay (next - now);
	}
}

int main ()
{
	TipoSistema sistema;
	unsigned int next;

	// Configuracion e inicializacion del sistema
	ConfiguraSistema (&sistema);
	InicializaSistema (&sistema);

	fsm_trans_t reproductor[] = {
		{ WAIT_END_MELODIA, CompruebaFinalEfecto, MELODIA_INICIO, IniciaMelodia },
		{ WAIT_END_MELODIA, CompruebaNuevaNota, MELODIA_INICIO, ComienzaNuevaNota},
		{ MELODIA_INICIO, CompruebaNotaTimeout, WAIT_END_MELODIA, ActualizaPlayer },
		{ MELODIA_INICIO, CompruebaStartGame, WAIT_START, SilenciaMelodia },
		{ WAIT_START, CompruebaStartDisparo, WAIT_NEXT, InicializaPlayDisparo },
		{ WAIT_START, CompruebaStartImpacto, WAIT_NEXT, InicializaPlayImpacto },
		{ WAIT_NEXT, CompruebaStartImpacto, WAIT_NEXT, InicializaPlayImpacto },
		{ WAIT_NEXT, CompruebaNotaTimeout, WAIT_END, ActualizaPlayer },
		{ WAIT_END, CompruebaFinalEfecto, WAIT_START, FinalEfecto },
		{ WAIT_END, CompruebaNuevaNota, WAIT_NEXT, ComienzaNuevaNota},
		{-1, NULL, -1, NULL },
	};
	fsm_trans_t columns[] = {
		{ KEY_COL_1, CompruebaColumnTimeout, KEY_COL_2, col_2 },
		{ KEY_COL_2, CompruebaColumnTimeout, KEY_COL_3, col_3 },
		{ KEY_COL_3, CompruebaColumnTimeout, KEY_COL_4, col_4 },
		{ KEY_COL_4, CompruebaColumnTimeout, KEY_COL_1, col_1 },
		{-1, NULL, -1, NULL },
	};
	fsm_trans_t keypad[] = {
		{ WAIT_KEY, key_pressed, WAIT_KEY, process_key},
		{-1, NULL, -1, NULL },
	};
	fsm_trans_t torreta[] = {
		{ WAIT_START, CompruebaComienzo, WAIT_MOVE, ComienzaSistema },
		{ WAIT_MOVE, CompruebaJoystickUp, WAIT_MOVE, MueveTorretaArriba },
		{ WAIT_MOVE, CompruebaJoystickDown, WAIT_MOVE, MueveTorretaAbajo },
		{ WAIT_MOVE, CompruebaJoystickLeft, WAIT_MOVE, MueveTorretaIzquierda },
		{ WAIT_MOVE, CompruebaJoystickRight, WAIT_MOVE, MueveTorretaDerecha },
		{ WAIT_MOVE, CompruebaTriggerButton, TRIGGER_BUTTON, DisparoIR },
		{ TRIGGER_BUTTON, CompruebaTimeoutDisparo, WAIT_MOVE, FinalDisparoIR },
		{ TRIGGER_BUTTON, CompruebaImpacto, WAIT_MOVE, ImpactoDetectado },
		{ WAIT_MOVE, CompruebaFinalJuego, WAIT_END, FinalizaJuego },
		{-1, NULL, -1, NULL },
	};
	
	//Creacion de las maquinas de estado
	fsm_t* player_fsm = fsm_new (MELODIA_INICIO, reproductor, &(sistema.player));
	fsm_t* columns_fsm = fsm_new (KEY_COL_1, columns, &teclado);
	fsm_t* keypad_fsm = fsm_new (WAIT_KEY, keypad, &teclado);
	fsm_t* torreta_fsm = fsm_new (WAIT_START, torreta, &(sistema.torreta));

	IniciaMelodia (player_fsm);	//Comienza a reproducirse melodia de inicio

	next = millis();
	while (!flags_system) {
		fsm_fire (player_fsm);
		fsm_fire (columns_fsm);
		fsm_fire (keypad_fsm);
		fsm_fire (torreta_fsm);
		next += CLK_MS;
		delay_until (next);
	}

	fsm_destroy(player_fsm);	//Libera las maquina de estados
	fsm_destroy(columns_fsm);
	fsm_destroy(keypad_fsm);
	fsm_destroy(torreta_fsm);
	tmr_destroy(teclado.tmr_duracion_columna);		//Libera los timer
	tmr_destroy(sistema.player.tmr_notas);
	tmr_destroy(sistema.torreta.tmr_shoot);
	return 0;
}
