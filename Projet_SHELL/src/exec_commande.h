#ifndef __EXEC_COMMANDE__
#define __EXEC_COMMANDE__

#include <stdio.h>
#include <stdlib.h>
#include "readcmd.h"
#include "csapp.h"

void handler_SIGCHLD(int sig);

/*fonction commande(struct cmdline *l)
Entree : "l" la ligne de commande
Sortie : Renvoie un entier entre 0 et 3 selon l'entree de la fonction*/
int commande(struct cmdline *l);

/*fonction commande_externe(struct cmdline *l)
Entree : "l" la ligne de commande
    Gère les commande externe */
void commande_externe(struct cmdline *l,int num_commande);

/*fonction Quit()
    Si la commande quit est taper, quit le shell*/
void Quit();

/*fonction CD(struct cmdline *l)
Entree : "l" la ligne de commande
    Gère la commande cd*/
void CD(struct cmdline *l);


/* fonction exec_commande(struct cmdline *l)
Entree : "l" la ligne de commande
    Execute la commande selon la sortie de commande(struct cmdline *l)*/
void commande_externe(struct cmdline *l,int num_commande);

void gestion_redirection(struct cmdline *l, int inout);

void exec_commande(struct cmdline *l);

#endif