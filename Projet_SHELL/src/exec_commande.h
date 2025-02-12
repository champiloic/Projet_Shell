#ifndef __EXEC_COMMANDE__
#define __EXEC_COMMANDE__

#include <stdio.h>
#include <stdlib.h>
#include "readcmd.h"
#include "csapp.h"

/*fonction commande(struct cmdline *l)
    Renvoie 0 si la commande est quit
    Renvoie 1 si la commande esr cd
    Sinon renvoie 2 si c'est une fonction externe*/
int commande(struct cmdline *l);

/*fonction commande_externe(struct cmdline *l)
    Gère les commande externe */
void commande_externe(struct cmdline *l);

/*fonction Quit()
    Si la commande quit est taper, quit le shell*/
void Quit();

/*fonction CD(char* l)
    Gère la commande cd*/
void CD(struct cmdline *l);


/* fonction exec_commande(struct cmdline *l)
    Execute la commande selon la sortie de commande(struct cmdline *l)*/
void exec_commande(struct cmdline *l);

void gestion_redirection(struct cmdline *l);


#endif