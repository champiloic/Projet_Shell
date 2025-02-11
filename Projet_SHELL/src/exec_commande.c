#include <stdio.h>
#include <stdlib.h>
#include "readcmd.h"
#include "csapp.h"
#include "exec_commande.h"

void exec_commande(struct cmdline *l){
    if ( strcmp(l->seq[0][0],"quit") == 0 ){//quelque commande interne
        	exit(0);
   	}
    if(strcmp(l->seq[0][0],"cd") == 0){ 
        int error_cd = chdir(l->seq[0][1]);
        if(error_cd ==-1){
            perror("fork failed");
        }
    }
    else{ // commande externe
        pid_t pid = fork();
	    if(pid == 0){
		    int error = execvp(l->seq[0][0],l->seq[0]);
            if(error == -1){
                perror("exec");
            }
		    exit(0);
	    }
	    wait(NULL);
    }
}