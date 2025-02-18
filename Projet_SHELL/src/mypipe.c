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
        if (l->in) {
            gestion_redirection(l, 0);
            int err = execvp(l->seq[0][0],l->seq[0]);
            if(err == -1){
                perror("exec");
            }
        }
        else {
            int err = execvp(l->seq[0][0],l->seq[0]);
            if(err == -1){
                perror("exec");
            }
        }
        
        close(fd[1]);
        exit(0);
    }
    if(Fork() == 0){
        close(fd[1]);
        dup2(fd[0],0);
        if (l->out) {
            gestion_redirection(l, 1);
            int err = execvp(l->seq[1][0],l->seq[1]);
            if(err == -1){
                perror("exec");
            }
        }
        else {
            int err = execvp(l->seq[1][0],l->seq[1]);
            if(err == -1){
                perror("exec");
            }
        }
        
        close(fd[0]);
        exit(0);
    }
    close(fd[1]);
    close(fd[0]);
    wait(NULL);
    wait(NULL);

}
void fermeture(int pipes[][2], int nb_cmd, int curr) {
    // commande actuelle premiere du tableau -> on ne ferme que son entrée puis on ferme tout le reste
    if (curr == 0) {
        close(pipes[0][0]);
        for (int i = 1; i< nb_cmd-1; i++) {
            close(pipes[i][0]);
            close(pipes[i][1]);
        }
    }

    // commande actuelle dernière du tableau -> on ne ferme que sa sortie puis on ferme tout le reste

    else if (curr == nb_cmd-1) {
        close(pipes[nb_cmd-1][1]);
        for (int i =0; i<nb_cmd-2; i++) {
            close(pipes[i][0]);
            close(pipes[i][1]);
        }
    }

    else if (curr == -1) {
        for (int i =0; i<nb_cmd-1; i++) {
            close(pipes[i][0]);
            close(pipes[i][1]);
        }
    }
    // commande actuelle ni première ni dernière -> on garde son entrée ouverte, et la sortie de la précédente ouverte, puis on ferme tout le reste

    else {
        for (int i = 0; i< nb_cmd-1; i++) {
            if (i+1 == curr) {
                close(pipes[i][0]);
            }
            if (i == curr) {
                close(pipes[i][1]);
            }
            else {
                close(pipes[i][0]);
                close(pipes[i][1]);
            }
        }
    }
}
void multi_pipes(struct cmdline *l, int nb_cmd) {
    int error;
    int i=0;
    int pipes[nb_cmd-1][2];
    for(int j=0; j<nb_cmd-1; j++) {
        if (pipe(pipes[j]) == -1) {
            perror("pipe");
            exit(1);
        }
    }

    for(i; i<nb_cmd-1; i++) {
        if (i==0) {
            // redir in
            if (Fork()==0) {
                fermeture(pipes, nb_cmd, i);
                dup2(pipes[0][1], STDIN_FILENO);
                if (l->in) {
                    gestion_redirection(l, 0);
                    error = execvp(l->seq[0][0],l->seq[0]);
                    if(error == -1){
                        perror("exec");
                    }
                }
                else {
                    error = execvp(l->seq[0][0],l->seq[0]);
                    if(error == -1){
                        perror("exec");
                    }
                }
                close(pipes[i][1]);
            }

        }
        else if (i == nb_cmd-2) {
            // redir out
            if (Fork()==0) {
                fermeture(pipes, nb_cmd, i);
                dup2(pipes[i][0], STDOUT_FILENO);
                if (l->out) {
                    gestion_redirection(l, 1);
                    error = execvp(l->seq[i][0],l->seq[i]);
                    if(error == -1){
                        perror("exec");
                    }
                }
                else {
                    error = execvp(l->seq[i][0],l->seq[i]);
                    if(error == -1){
                        perror("exec");
                    }
                }
                close(pipes[i][0]);
            }
        }
        else {
            // ?
            if (Fork()==0) {
                fermeture(pipes, nb_cmd, i);
                dup2(pipes[0][1], 1);
                error = execvp(l->seq[i][0],l->seq[i]);
                if(error == -1){
                        perror("exec");
                }
                close(pipes[i-1][1]);
                close(pipes[i][0]);
            }
        }   
    }
    fermeture(pipes, nb_cmd, -1);
    for (int i=0; i<nb_cmd-1; i++) {
        wait(NULL);
    }

}