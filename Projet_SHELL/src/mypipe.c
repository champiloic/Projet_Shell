#include <stdio.h>
#include <stdlib.h>
#include "readcmd.h"
#include "csapp.h"
#include "exec_commande.h"
#include "mypipe.h"

/********************************************/
int taille_seq(struct cmdline *l){
    int cpt=0;
    while (l->seq[cpt] != 0){
        cpt++;
    }
    return cpt;
}

/********************************************/

void one_pipe (struct cmdline *l){
    int error;
    int fd[2];
    pipe(fd);
    if(Fork() == 0){
        close(fd[0]);
        dup2(fd[1],1);
        error = execvp(l->seq[0][0],l->seq[0]);
        if(error == -1){
            perror("exec");
        }
        close(fd[1]);
        exit(0);
    }
    if(Fork() == 0){
        close(fd[1]);
        dup2(fd[0],0);
        int err = execvp(l->seq[1][0],l->seq[1]);
        if(err == -1){
            perror("exec");
        }
        close(fd[0]);
        exit(0);
    }
    close(fd[1]);
    close(fd[0]);
    wait(NULL);
    wait(NULL);

}