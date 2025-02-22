#ifndef __EXEC_COMMANDE__
#define __EXEC_COMMANDE__

#include <stdio.h>
#include <stdlib.h>
#include "readcmd.h"
#include "csapp.h"

/*Handler defini pour le signal SIGCHLD*/
void handler_SIGCHLD(int sig);

/*fonction Quit()
    Si la commande quit est taper, quit le shell*/
void Quit();

/*fonction CD(struct cmdline *l)
Entree : "l" la ligne de commande
    Gère la commande cd*/
void CD(struct cmdline *l);

/* fonction gestion_redirection(struct cmdline *l, int inout)
Entree : "l" la ligne de commande et un entier comprit dans [0,1] representant l'entree/sortie 
    Gère les redirections d'entrée/sortie */
void gestion_redirection(struct cmdline *l, int inout);

/*fonction commande_externe(struct cmdline *l)
Entree : "l" la ligne de commande
    Gère les commande externe */
void commande_externe(struct cmdline *l,int num_commande);

/*fonction commande(struct cmdline *l)
Entree : "l" la ligne de commande
Sortie : Renvoie un entier entre 0 et 3 selon l'entree de la fonction*/
int commande(struct cmdline *l);

/*fonction exec_commande(struct cmdline *l)
Entree: "l" la ligne de commande 
    Selon l'entier renvoyé par la fonction "commande" execute sa fonction attribuer */
void exec_commande(struct cmdline *l);

#endif