#include "CamaraController.h"

void *start(){
    ppid = fork();
    execv(START);
}
void *end(){
    kill(pid,SIGKILL);
}