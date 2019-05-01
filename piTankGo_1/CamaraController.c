/** File Name          : CamaraController.c
  * Description        : Cuerpo del controlador de la camara
  */

/* Includes ------------------------------------------------------------------*/
#include "CamaraController.h"

/**
 * @brief			Funcion que inicia la difusion de video
 * @note			START es el comando a ejecutar. Está definido en la cabecera
 */

void *start(){
    system(START);
    return 0;
}


/**
 * @brief			Funcion que acaba la difusion de video
  * @note			END es el comando a ejecutar. Está definido en la cabecera
 */
void *end(){
    system(END);
	return 0;
}

/**
 * @brief			Funcion que muestra por pantalla la interfaz y la ip de la raspi
 * @note			Imprime un error si no hay ninguna ip asignada o no se puede recuperar
 */
void *getIP(){
	/* Definimos variables locales. Las 2 primeras
	son listas linkeadas de structs de la librería 
	que contienen campos donde guardamos informacion sobre la red */

	struct ifaddrs *ifaddr, *ifa;
	int s; //Resultado de getnameinfo
	char host[NI_MAXHOST]; //hacemos hueco para la IP
		/* Obtenemos la lista */
	    if (getifaddrs(&ifaddr) == -1)
	    {
			/*Si no se puede recuperar imprimimos
			por la consola de error y terminamos la funcion*/
	        perror("getifaddrs"); 
	        return 0;
	    }

		/* Recorremos la lista */
	    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
	    {
			/* Si la direccion es nula pasamos a la siguiente iteracion del bucle */
	        if (ifa->ifa_addr == NULL)
	            continue;
			/*Traducimos la direccion socket*/
	        s=getnameinfo(ifa->ifa_addr,sizeof(struct sockaddr_in),host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
			/* Si el struct corresponde a la interfaz wifi */
	        if((strcmp(ifa->ifa_name,"wlan0")==0)&&(ifa->ifa_addr->sa_family==AF_INET))
	        {
				/* Si la traduccion ha dado error salimos */
	            if (s != 0)
	            {
	                printf("getnameinfo() failed: %s\n", gai_strerror(s));
	                exit(EXIT_FAILURE);
	            }
				/* Imprimimos interfaz e IP por pantalla */
	            printf("\tInterfaz : <%s>\n",ifa->ifa_name );
	            printf("\t  IP de la torreta: : <%s>\n", host);
	        }
	    }
	    fflush(stdout);
		/* Liberamos las direcciones de la lista linkeada */
	    freeifaddrs(ifaddr);
	    return 0;
}
