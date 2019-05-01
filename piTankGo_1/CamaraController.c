#include "CamaraController.h"

void *start(){
    system(START);
    return 0;
}
void *end(){
    system(END);
	return 0;
}
void *getIP(){
	struct ifaddrs *ifaddr, *ifa;
	    int s;
	    char host[NI_MAXHOST];

	    if (getifaddrs(&ifaddr) == -1)
	    {
	        perror("getifaddrs");
	        return 0;
	    }


	    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
	    {
	        if (ifa->ifa_addr == NULL)
	            continue;

	        s=getnameinfo(ifa->ifa_addr,sizeof(struct sockaddr_in),host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

	        if((strcmp(ifa->ifa_name,"wlan0")==0)&&(ifa->ifa_addr->sa_family==AF_INET))
	        {
	            if (s != 0)
	            {
	                printf("getnameinfo() failed: %s\n", gai_strerror(s));
	                exit(EXIT_FAILURE);
	            }
	            printf("\tInterfaz : <%s>\n",ifa->ifa_name );
	            printf("\t  IP de la torreta: : <%s>\n", host);
	        }
	    }
	    fflush(stdout);
	    freeifaddrs(ifaddr);
	    return 0;
}
