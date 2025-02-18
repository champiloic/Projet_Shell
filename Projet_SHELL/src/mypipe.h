#ifndef __MYPIPE__
#define __MYPIPE__

#include <stdio.h>
#include <stdlib.h>
#include "readcmd.h"
#include "csapp.h"
#include "exec_commande.h"
#include "mypipe.h"

/* taille_seq 
Entree: "l" la ligne de commande 
Sortie : Retourne la taille de la sequence */
int taille_seq(struct cmdline *l);

void one_pipe (struct cmdline *l);

void multi_pipes(struct cmdline *l, int nb_cmd);

void fermeture(int pipes[][2], int nb_cmd, int curr);



#endif