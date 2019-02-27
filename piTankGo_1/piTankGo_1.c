
#include "piTankGo_1.h"


int frecuenciasDisparo[16] = {2500,2400,2300,2200,2100,2000,1900,1800,1700,1600,1500,1400,1300,1200,1100,1000};
int tiemposDisparo[16] = {75,75,75,75,75,75,75,75,75,75,75,75,75,75,75,75};
int frecuenciasImpacto[32] = {97,109,79,121,80,127,123,75,119,96,71,101,98,113,92,70,114,75,86,103,126,118,128,77,114,119,72};
int tiemposImpacto[32] = {10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10};

pthread_t *thread_explora_teclado=NULL;	//Manejador de hebra que explora teclado

volatile int flags_juego = 0;
volatile int flags_system = 0;
volatile int flags_player = 0;
static TipoTeclado teclado;


//------------------------------------------------------
// FUNCIONES DE CONFIGURACION/INICIALIZACION
//------------------------------------------------------

// int ConfiguracionSistema (TipoSistema *p_sistema): procedimiento de configuracion del sistema.
// Realizará, entra otras, todas las operaciones necesarias para:
// configurar el uso de posibles librerías (e.g. PiGPIO),
// configurar las interrupciones externas asociadas a los pines GPIO,
// configurar las interrupciones periódicas y sus correspondientes temporizadores,
// crear, si fuese necesario, los threads adicionales que pueda requerir el sistema
int ConfiguraSistema (TipoSistema *p_sistema) {
	piLock(SYSTEM_FLAGS_KEY);
	piLock(STD_IO_BUFFER_KEY);
		// configura PiGPIO
		/*if (gpioInitialise() < 0) {
			printf ("No se pudo configurar PiGPIO\n");
			piUnlock (STD_IO_BUFFER_KEY);
			return 1;
	    }*/
		// Configurar los pines utilizando las variables definidas en PiTankGoLib.h
		// ...
	// configura wiringPi
			if (wiringPiSetup () < 0) {
				printf ("No se pudo configurar wiringPi\n");
			}
	ConfiguraPins();
	piUnlock(STD_IO_BUFFER_KEY);
	piUnlock(SYSTEM_FLAGS_KEY);
	return 0;
}
void ConfiguraPins(){
	//Configura salida sonidos
	pinMode (PLAYER_PWM_PIN, OUTPUT);
	softToneCreate(IR_TX_PIN);
	//Configuramos teclado
	if(IniciaInOutTeclas()<0){
		printf ("No se pudo configurar el teclado\n");
	};


}



// int InicializaSistema (TipoSistema *p_sistema): procedimiento de inicializacion del sistema.
// Realizará, entra otras, todas las operaciones necesarias para:
// la inicializacion de los diferentes elementos de los que consta nuestro sistema,
// la torreta, los efectos, etc.
// igualmente arrancará el thread de exploración del teclado del PC
int InicializaSistema (TipoSistema *p_sistema) {
	piLock(SYSTEM_FLAGS_KEY);
	int result = 0;
	result = piThreadCreate (thread_explora_teclado_PC);
/*
	piLock (STD_IO_BUFFER_KEY);
	thread_explora_teclado = gpioStartThread (thread_explora_teclado_PC,"\0");*/
	piLock(PLAYER_FLAGS_KEY);
	flags_player=0;
	flags_juego = 0;
	flags_system = 0;
	piUnlock(PLAYER_FLAGS_KEY);
	/*if (thread_explora_teclado == NULL) {*/
	 if (result != 0) {
		printf ("No empieza!!!\n");
		piUnlock (STD_IO_BUFFER_KEY);
		return -1;
	}

	char * nombre_disparo="despacito";
	char * nombre_impacto="starwars";

	TipoPlayer *player=&(p_sistema->player);
	printf("\nBIENVENIDO A SU TORRETA SOLDADO\n");
	printf("a:COMIENZA DISPARO\t s:SIGUIENTE NOTA\t d:START IMPACTO\tf:TERMINAR PROGRAMA\n");
			fflush(stdout);
	if(InicializaEfecto(&(player->efecto_disparo),nombre_disparo,frecuenciasDisparo,tiemposDisparo,16)<1){
		printf("\n[ERROR!!!][InicializaEfecto]\n");
		fflush(stdout);
	}
	if(InicializaEfecto(&(player->efecto_impacto),nombre_impacto,frecuenciasImpacto,tiemposImpacto,32)<1){
		printf("\n[ERROR!!!][InicializaEfecto]\n");
		fflush(stdout);
	}

	InicializaPlayer(player);
	InicializaTorreta(&p_sistema->torreta);
	//p_sistema->torreta;
	p_sistema->debug=0;	// Modo traza desactivado
	p_sistema->teclaPulsada = '\0';

	piUnlock (STD_IO_BUFFER_KEY);
	piUnlock(SYSTEM_FLAGS_KEY);
	return result;
}

//------------------------------------------------------
// SUBRUTINAS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------
PI_THREAD (thread_explora_teclado_PC) {
//void *thread_explora_teclado_PC(void *arg) {	//Rutina de la hebra explorar teclado
	int teclaPulsada;

	while(1) {
		delay(10); // Wiring Pi function: pauses program execution for at least 10 ms

		piLock (STD_IO_BUFFER_KEY);

		if(kbhit()) {
			teclaPulsada = kbread();


			switch(teclaPulsada) {
				
				case 'a':
					piLock (PLAYER_FLAGS_KEY);
					flags_player |= FLAG_START_DISPARO;
					piUnlock (PLAYER_FLAGS_KEY);

					printf("\nTecla EMPEZAR pulsada!\n");
					fflush(stdout);
					break;
				/*
				    case 's':
					piLock (PLAYER_FLAGS_KEY);
					flags_player |= FLAG_NOTA_TIMEOUT;
					piUnlock (PLAYER_FLAGS_KEY);

					printf("\nTecla SIGUIENTE NOTA pulsada!\n");
					fflush(stdout);
					break;
				*/
				case 'd':
					piLock (PLAYER_FLAGS_KEY);
					flags_player |= FLAG_START_IMPACTO;
					piUnlock (PLAYER_FLAGS_KEY);

					printf("\nTecla MELODIA IMPACTO pulsada!\n");
					fflush(stdout);
					break;
				/* Definir tecla para terminar el juego y que ejecute:
				 * gpioStopThread(thread_explora_teclado);
				 * gpioTerminate();
				 */
				case 'f':
					piLock (PLAYER_FLAGS_KEY);
					flags_player |= FLAG_PLAYER_STOP;
					flags_system = 1;
					piUnlock (PLAYER_FLAGS_KEY);

					printf("\nTecla FLAG_PLAYER_STOP pulsada!\n");
					fflush(stdout);
					break;

				default:
					printf("\nINVALID KEY!!!\n");
					break;
			}
		}

		piUnlock (STD_IO_BUFFER_KEY);
	}
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
		{ KEY_WAITING, key_pressed, KEY_WAITING, process_key},
		{-1, NULL, -1, NULL },
	};
	//Creacion de las maquinas de estado
	fsm_t* player_fsm = fsm_new (WAIT_START, reproductor, &(sistema.player));	// Crea e inicia la maquina de estados
	fsm_t* columns_fsm = fsm_new (KEY_COL_1, columns, &teclado);
	fsm_t* keypad_fsm = fsm_new (KEY_WAITING, keypad, &teclado);
	next = millis();
	while (!flags_system) {
		fsm_fire (player_fsm);
		fsm_fire (columns_fsm);
		fsm_fire (keypad_fsm);
		next += CLK_MS;
		delay_until (next);
	}

	//gpioStopThread(thread_explora_teclado);
	fsm_destroy (player_fsm);
	fsm_destroy (columns_fsm);
	fsm_destroy (keypad_fsm);	//Libera las maquina de estados
	//gpioTerminate();
	return 0;
}
