#include <stdio.h>
#include <stdlib.h>
#include "readcmd.h"
#include "csapp.h"
#include "exec_commande.h"
/******************************************************************/
int commande(struct cmdline *l){
    if ( strcmp(l->seq[0][0],"quit") == 0 ){//quelque commande interne
        return 0;
   	}
    else if(strcmp(l->seq[0][0],"cd") == 0){ 
        return 1;
    }
    else if(!l->in || !l->out){
        return 2;
    }
    else{
        return 3;
    }
};

/******************************************************************/
void Quit(){
    exit(0);
}

/******************************************************************/
void CD(struct cmdline *l){
    int error_cd = chdir(l->seq[0][1]);
    if(error_cd ==-1){
        perror("cd failed");
    }
}

/******************************************************************/
void commande_externe(struct cmdline *l){// commande externe
    pid_t pid = fork();
	if(pid == 0){
		int error = execvp(l->seq[0][0],l->seq[0]);
        if(error == -1){
            perror("exec");
        }
		exit(0);
	}
	wait(NULL);
}

/******************************************************************/
void exec_commande(struct cmdline *l){
    switch(commande(l)){
        case(0):
                Quit();
        case(1):
                CD(l);
                break;
        case(2):
                gestion_redirection(l);
                commande_externe(l);
                break;
        case(3):
                commande_externe(l);
                break;
        default:
            printf("commande inconnue");
    }
}

/******************************************************************/

void gestion_redirection(struct cmdline *l){
    if (l->in != NULL){
        int fd_in = open(l->in,O_RDWR,0) ; //decripteur de fichier en cas de redirection d'entree 
                
        // Gestion des erreurs lors des redirections 
                
        if (access(l->in, F_OK) != 0){      // Verification si le fichier existe avec la primitive acces
            printf("%s :file not found \n",l->in);
            exit(0);
        }

        else if (access(l->in, R_OK) != 0){   // Verification des droits de lecture   
            printf("%s: Permission denied\n",l->in);
            exit(0);
        }
                
        else {
            dup2(fd_in,0);   // mettre une copie de fd dans l'entre standard
                
        }
        close(fd_in);
                
    }
    if (l->out != NULL){
        int fd_out = open(l->out,O_RDWR,0) ; //decripteur de fichier en cas de redirection de sortie 
                
        if (access(l->out, F_OK) != 0){      // Verification si le fichier existe avec la primitive acces
            printf("%s :file not found \n",l->out);
            exit(0);
        }

        else if (access(l->out, W_OK) != 0){   // Verification des droits de lecture   
            printf("%s: Permission denied\n",l->out);
            exit(0);
        }
        else {
            dup2(fd_out,1);   // mettre une copie de fd dans la sortie standard
                
        }
        close(fd_out);
    }

}