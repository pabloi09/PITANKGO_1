#include "CamaraController.h"

void *start(){
    pid = fork();
    system(START);
    return 0;
}
void *end(){
    system(END);
	return 0;
}
