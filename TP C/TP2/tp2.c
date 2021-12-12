#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>

#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>

#include<unistd.h>

void partie1(){

    printf("\nPartie 1 :\n\n");

	pid_t f;
	int status;

	f=fork();

	if (f == 0) {
		printf("Contexte fils\n Processus courant (PID) : %d\n Processus père (PPID) : %d\n\n", getpid(), getppid());
		int codeRetourFils = getpid() % 10;
		exit(codeRetourFils);
	}
	else {
		printf("Contexte père\n Processus courant (PID) : %d\n Processus fils : %d\n\n", getpid(), f);
		wait(&status);

		if(WIFEXITED(status)){
				printf("Code retour du processus fils : %d\n", WEXITSTATUS(status));
		}
	}

	
}


void partie2(char *a)
{
    printf("\nPartie 2 :\n\n");

	printf("Votre message : %s\n\n", a);

	pid_t f;
	f=fork();

	// buffer qui contient le fichier temporaire
    char nameBuff[32];
    // buffer qui contient les données à écrire/lire dans le fichier temporaire
    char buffer[24];

	// initialisation du fil descripteur
    int filedes = -1;

	int status;
	

	// memset les buffers à 0
    memset(nameBuff,0,sizeof(nameBuff));
    memset(buffer,0,sizeof(buffer));

    // Copy les infos dans les buffers
    strncpy(nameBuff,"/tmp/proc-XXXXXX",21);
    strncpy(buffer,"Hello World",11);

	errno = 0;

	if (f == 0) { //FILS

		printf("Contexte fils\n Processus courant (PID) : %d\n", getpid());
		
		close(1);

		// Creer le fichier temporaire en remplacer les X
		filedes = mkstemp(nameBuff);

		// Appel unlink pour qu'à chaque fois que le fichier est fermé ou que le programme se termine
		// supprime le fichier temporaire
		unlink(nameBuff);
		

		if(filedes<1)
		{
			printf("\n Echec de la creation du fichier temporaire avec l'erreur [%s]\n",strerror(errno));
			
		}
		else
		{
			printf(" Fichier temporaire [%s] : créé avec succès", nameBuff);
			printf("\n Valeur du descripteur :  %d\n", filedes);
		}

	}
	else{
		//Affiche son Pid
		printf("Contexte père\n Processus courant (PID) : %d\n\n", getpid());

		//Attends que le fils fini
		wait(&status);

		if(WIFEXITED(status)){
				printf("\nThat’s All Folks !\n" );
		}
	}
}

int partie3()
{
	printf("\nPartie 3 |\n\n");
	/*Redirection de flux via PIPE*/ 

	/* 
	 * Parametre de la fonction execlp
	 * 	---ps---
	*/
	char *file_name_ps = "ps";
	char *arg1_ps = "-e";
	char *arg2_ps = "-a";
	char *arg3_ps = "-u";
	char *arg4_ps = "-x";

	/* 
	 * Parametre de la fonction execlp
	 * 	---grep---
	*/
	char *file_name_grep = "grep";
	char *arg1_grep = "^root" ;
	

	// Declaration d'un descripteur 
	int fd[2];						/*[0] : descripteur en lecture*/  /*[1] : descripteur en ecriture*/ 

	// Creation du tube anonyme
	if(pipe(fd) == -1)
	{
		return 1;
	}

	// Utilisation d'un fork 1
	int pid1 = fork(); 				//ps prend la valeur de retour du fork()
									//[0] : processus fils
									//1] : processus pere

	if(pid1 < 0)
	{
		return 2;
	}
	

	// Traitement du fils
	if(pid1 == 0)
	{
		int devNull = open("/dev/null", O_WRONLY);

		if(devNull == -1)
	  	{
		    fprintf(stderr,"Error in open('/dev/null',0)\n");
		    exit(EXIT_FAILURE);
	    }
		// Dupplication du file descripteur en ecriture
		//dup2(fd[1], STDOUT_FILENO);
		int dup2Result = dup2(devNull, STDOUT_FILENO);
		if(dup2Result == -1) 
		{
        	fprintf(stderr,"Error in dup2(devNull, STDOUT_FILENO)\n");
        	exit(EXIT_FAILURE);
    	}
		// Fermeture du file descripteur en lecture et ecriture, pour la dupplication 1
		close(fd[0]);
		close(fd[1]); // On le ferme pck il a etait duppliqué (pour ne pas en avoir deux d'ouvert

		//***ps***
		execlp(file_name_ps, file_name_ps, arg1_ps, arg2_ps, arg3_ps, arg4_ps, NULL);
	}

	// Traitement du pere (programme principal) 

	// Utilisation d'un fork 2
	int pid2 = fork(); 				/*ps prend la valeur de retour du fork()*/
									/*[0] : processus fils*/  
									/*[1] : processus pere*/

	if(pid2 < 0)
	{
		return 3;
	}

	if(pid2 == 0)
	{
		// Dupplication en lecture
		dup2(fd[0], STDIN_FILENO);
		int f = open("/dev/null", O_WRONLY);
		dup2(f, 1);
		//Fermeture des fd de la dupplication 2
		close(fd[0]);
		close(fd[1]);

		//***grep***
		execlp(file_name_grep, file_name_grep, arg1_grep, NULL);
	}

	// On ferme les fd courants
	close(fd[0]);
	close(fd[1]);

	//Attends que le fils fini (pour le 1 et le 2)
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);

	if(WIFEXITED(pid2) == 0){
		printf("root est connecté");
	}
	

	return 0;

}

/*
argv est le nombre d'argument + 1
argv[x] contient l'argument numero x
argv[0] contient le nom du programme, chemin y compris.
*/

int main(int argc, char** argv) // ou (int argc, char argv[])
{
	printf("------------------------------------");
	partie1();
	printf("------------------------------------\n\n");

	if(argc<1)
	{
		printf("Erreur Arg\n Vous devez entrer au moin 1 argument");
	}
	
	if(argv[1] == NULL)
	{
		printf("------------------------------------\n");
		printf("Partie 2 | Erreur : Vous devez entrer un mot\n");
		printf("------------------------------------\n\n");
	}
	else
	{
		printf("------------------------------------\n");
		partie2(argv[1]);	
		printf("------------------------------------\n\n");
	}

	printf("------------------------------------\n");
	partie3();
	printf("------------------------------------\n\n");
	
}
