#include <stdio.h>
#include <stdlib.h>
#include "readcmd.h"
#include "csapp.h"
#include "exec_commande.h"

void exec_commande(cmdline *l){
    if(strcmp(l->seq[0][0],"cd")){ //quelque commande interne

    }
    if(strcmp(l->seq[0][0],"mkdir")){

    }
    else{ // commande externe
        pid_t pid = fork();
	    if(pid == 0){
		    execvp(l->seq[0][0],l->seq[0]);
		    exit(0);
	    }
	    wait(NULL);
    }
}