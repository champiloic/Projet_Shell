#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "readcmd.h"
#include "csapp.h"
#include "exec_commande.h"
#include "mypipe.h"

/********************************************/
int taille_seq(struct cmdline *l) {
    int cpt = 0;
    while (l->seq[cpt] != 0) {
        cpt++;
    }
    return cpt;
}
/********************************************/

void one_pipe(struct cmdline *l) {
    int error;
    int fd[2];
    pipe(fd);
    if (Fork() == 0) {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        if (l->in) {
            gestion_redirection(l, 0);
        }
        printf("Executing: %s\n", l->seq[0][0]);
        execvp(l->seq[0][0], l->seq[0]);
        perror("exec");
        exit(EXIT_FAILURE);
    }
    if (Fork() == 0) {
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        if (l->out) {
            gestion_redirection(l, 1);
        }
        printf("Executing: %s\n", l->seq[1][0]);
        execvp(l->seq[1][0], l->seq[1]);
        perror("exec");
        exit(EXIT_FAILURE);
    }
    close(fd[1]);
    close(fd[0]);
    wait(NULL);
    wait(NULL);
}


void multi_pipes(struct cmdline *l, int nb_cmd) {
    int pipes[nb_cmd - 1][2];  // Tubes entre les commandes

    // Création des nb_cmd-1 tubes
    for (int i = 0; i < nb_cmd - 1; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(1);
        }
    }

    // Création des processus fils
    for (int i = 0; i < nb_cmd; i++) {
        if (fork() == 0) {  // Processus fils
            // Fermeture des tubes inutiles
            for (int j = 0; j < nb_cmd - 1; j++) {
                if (j != i - 1 && j != i) {
                    close(pipes[j][0]);
                    close(pipes[j][1]);
                }
            }

            // Gestion des entrées/sorties des commandes
            if (i > 0) {  // Commande intermédiaire ou dernière
                close(pipes[i - 1][1]);  // Ferme l'extrémité d'écriture du tube précédent
                dup2(pipes[i - 1][0], STDIN_FILENO);  // Redirige l'entrée standard
                close(pipes[i - 1][0]);  // Ferme l'extrémité de lecture du tube précédent
            }
            if (i < nb_cmd - 1) {  // Pas la dernière commande
                close(pipes[i][0]);  // Ferme l'extrémité de lecture du tube courant
                dup2(pipes[i][1], STDOUT_FILENO);  // Redirige la sortie standard
                close(pipes[i][1]);  // Ferme l'extrémité d'écriture du tube courant
            }

            // Exécution de la commande
            if (execvp(l->seq[i][0], l->seq[i]) == -1) {
                perror("exec");
                exit(EXIT_FAILURE);  // Terminate child process on exec failure
            }
        }
    }

    // Fermeture des tubes dans le processus parent
    for (int i = 0; i < nb_cmd - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Attendre tous les processus fils
    for (int i = 0; i < nb_cmd; i++) {
        wait(NULL);
    }
}