/*
 * SerialReader.c
 *
 *  Created on: 22 abr. 2019
 *      Author: diego
 */
#include "SerialReader.h"

uint8_t mensaje[MSGSIZE+1];	//String que guarda las palabras recibidas más el caracter \0 al final
int buff;	//Variable para leer cada byte recibido


/* void joystick(): rutina de atencion al puerto serie.
 * Funcion llamada en ComienzaSistema() de torreta.c
 */
void *joystick () {

    int Serial = serialOpen (USB, SPEED);			//Abrimos la conexion serie
    if(Serial<0){printf(NO_CONNECT);return NULL;}	//Mensaje de error

    while(!(flags_juego & FLAG_SYSTEM_END)){		/*La conexion dura mientras dure el juego*/

    	memset(mensaje, 0, sizeof(mensaje));	//Limpiamos el string
    	int check=0;	//Variable de control de fallos

    	while(serialDataAvail( Serial )<MSGSIZE);	//Esperamos a que se cargue el buffer
    	for(int i=0; i<MSGSIZE; i++) {
    		buff=serialGetchar( Serial );	//Leemos cada byte del mensaje
    	    if(i==0 && buff!=bsync){		//Condicion: El primer byte no es el de sincronismo
    	        check=-1;
    	        serialFlush( Serial );
    	        break;
    	    }
    	    else if(buff>=0){				//Condicion: El byte leido es valido=>lo añadimos al mensaje
    	        mensaje[i]=(uint8_t)buff;
    	    }
    	    else{
    	        check=-1;
    	        serialFlush( Serial );
    	        break;
    	    }
    	}

    	if(check==0 && mensaje[0]==bsync){
    		if(0 == strcmp((char *)mensaje, RIGHT)){
    				piLock (GAME_FLAGS_KEY);
    				flags_juego |= FLAG_JOYSTICK_RIGHT;
    				piUnlock (GAME_FLAGS_KEY);
    		}else if(0 == strcmp((char *)mensaje, LEFT)){
    				piLock(GAME_FLAGS_KEY);
    				flags_juego |= FLAG_JOYSTICK_LEFT;
    				piUnlock (GAME_FLAGS_KEY);
    		}else if(0 == strcmp((char *)mensaje, UP)){
    				piLock (GAME_FLAGS_KEY);
    				flags_juego |= FLAG_JOYSTICK_UP;
    				piUnlock (GAME_FLAGS_KEY);
    		}else if(0 == strcmp((char *)mensaje, DOWN)){
    				piLock (GAME_FLAGS_KEY);
    				flags_juego |= FLAG_JOYSTICK_DOWN;
    				piUnlock (GAME_FLAGS_KEY);
    		}else{
        	        serialFlush( Serial );
    				piLock (STD_IO_BUFFER_KEY);
    				printf("No se reconoce el comando: %s\n",mensaje);
    				piUnlock (STD_IO_BUFFER_KEY);
    		}
    	}
    	else{
    		printf("\n\nSerial Comm Failure\n");
    	}

    }

    serialClose(Serial);
    return NULL;
}
