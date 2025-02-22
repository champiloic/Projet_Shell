/*
 * Copyright (C) 2002, Simon Nieuviarts
 */

#include <stdio.h>
#include <stdlib.h>
#include "readcmd.h"
#include "csapp.h"
#include "exec_commande.h"


int main()
{
	while (1) {
		struct cmdline *l;
		// int i, j;

		printf("shell> ");
		l = readcmd();

		/* If input stream closed, normal termination */
		if (!l) {
			printf("exit\n");
			exit(0);
		}
		else if (l->err) {
			/* Syntax error, read another command */
			printf("error: %s\n", l->err);
			
		}
		else if(l->seq[0] == NULL){ // si la commande taper est vide
            continue;
        }

		else {
			exec_commande(l);
		}

	}
}
