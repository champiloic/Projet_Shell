#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "readcmd.h"
#include "csapp.h"
#include "exec_commande.h"

int commande(struct cmdline *l){
    if(taille_seq(l)>1){
        //printf("%d\n",taille_seq(l));
        return 3 ;
    }
    else if ( strcmp(l->seq[0][0],"quit") == 0 ){     //quelque commande interne
        return 0;
   	}
    else if(strcmp(l->seq[0][0],"cd") == 0){ 
        return 1;
    }
    else {                                      //commande externe + redirection
        return 2;
    }
    
};

void Quit() {
    exit(0);
}

void CD(struct cmdline *l) {
    if (l->seq[0][1] == NULL) {
        if (chdir(getenv("HOME")) == -1) {
            perror("cd to home failed");
        }
    } else {
        if (chdir(l->seq[0][1]) == -1) {
            perror("cd failed");
        }
    }
}


void gestion_redirection(struct cmdline *l, int inout) {
    if (inout==0) {
        int fd_in = open(l->in, O_RDONLY);
        if (fd_in == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }
        dup2(fd_in, STDIN_FILENO);
        close(fd_in);
    }

    if (inout==1) {
        int fd_out = open(l->out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd_out == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }
        dup2(fd_out, STDOUT_FILENO);
        close(fd_out);
    }
    if (inout==2) {
    }

    else {
        perror("inout");
    }
}

void commande_externe(struct cmdline *l, int num_commande){// commande externe
    pid_t pid = Fork();
	if(pid == 0){
        int inout=2;
        if (l->in) {
            inout=0;
        }
        else if (l->out) {
            inout=1;
        }
        gestion_redirection(l,inout);
    
		int error = execvp(l->seq[num_commande][0],l->seq[num_commande]);
        if(error == -1){
            perror("exec");
            exit(1);
        }
		exit(0);
	}
    else if (pid > 0){
	    wait(NULL);
    }
    else{
        perror("fork");
        exit(1);
    }
}


void exec_commande(struct cmdline *l){
    switch(commande(l)){
        case(0):
                Quit();
        case(1):
                CD(l);
                break;
        case(2):
                commande_externe(l,0);
                break;
        case(3):
            if (taille_seq(l)==2) {
                one_pipe(l);
            }  
            else {
                multi_pipes(l, taille_seq(l));
            }
                break;
        default:
            printf("commande inconnue\n");
    }
}
