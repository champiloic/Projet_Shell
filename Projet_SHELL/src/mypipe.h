#ifndef __MYPIPE__
#define __MYPIPE__

#include <stdio.h>
#include <stdlib.h>
#include "readcmd.h"
#include "csapp.h"
#include "exec_commande.h"
#include "mypipe.h"

/* taille_seq (struct cmdline *l)
Entree: "l" la ligne de commande 
Sortie : Retourne la taille de la sequence */
int taille_seq(struct cmdline *l);

/*one_pipe (struct cmdline *l)
Entree: "l" la ligne de commande
Fonction qui gère 2 commande avec un pipe
*/
void one_pipe (struct cmdline *l);

/*multi_pipes(struct cmdline *l, int nb_cmd)
Entree: "l" la ligne de commande , nb_cmd la nombre de commande
Fonction qui gère n commande avec n-1 pipes
*/
void multi_pipes(struct cmdline *l, int nb_cmd);

#endif