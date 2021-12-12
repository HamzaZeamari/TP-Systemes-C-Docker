#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>

/**
 * L'objectif de ce code est de mettre en place d'un processus :
 * ===
 * - générant deux processus fils de même niveau hiérarchique.
 * - réalisant une communication ascendante fils -> père via les tubes.
 * ===
 * Schématiquement :
 * ===
 *       Père
 *       ^ ^
 *       | | 
 * Fils1 | | Fils2
 */
int main(int argc, char** argv) {
  int tube_1[2];

  /**
   * La création du tube à ce stade permet de rendre disponible
   * le medium de communication au sein de n'importe quel fils 
   * (généré par un appel fork())
   */
  if (pipe(tube_1) < 0) {
    perror("Unable to open a pipe!");
    exit(1);
  }

  // Création du FILS N°1 via fork()
  if (fork() == 0) {
    // Code réalisé par le FILS1 (valeur retour de fork() == 0)
    /**
     * Fermeture des descripteurs non utilisés :
     * communication ascendante fils->père = fermeture du descripteur de lecture (indice 0)
     */
    close(tube_1[0]);	
    // Ecriture de données dans le tube.
    write(tube_1[1], "FILS1", sizeof(char)*5);
    // Affichage du PID du processus
    printf("[FILS 1] pid :%d\n", getpid()); 
    // Terminaison du nominal processus via code retour = 0;
    exit(0);
  }
  else {
    // Code réalisé par le père (valeur retour de fork() != 0)
    // Création du FILS N°2 via fork()
    if (fork() == 0) {
      // Code réalisé par le FILS2
      /**
       * Fermeture des descripteurs non utilisés :
       * communication ascendante fils->père = fermeture du descripteur de lecture (indice 0)
       */
      close(tube_1[0]);
      // Ecriture de données dans le tube.
      write(tube_1[1], "FILS2", sizeof(char)*5);
      // Affichage du PID du processus
      printf("[FILS 2] pid :%d\n", getpid());
      // Terminaison du nominal processus via code retour = 0;
      exit(0);
    }
    else {
      // Code réalisé par le père (valeur retour de fork() != 0)
      close(tube_1[1]);

      int i = 0;
      char buffer;
      // Lecture de toute donnée disponible dans le tube.
      while(read(tube_1[0], &buffer, sizeof(char)) != 0) {
        printf("%c\n", buffer);
      }

      // Attende de terminaison des 2 processus fils pour ne pas générer des processus zombies.
      wait(NULL);    
      wait(NULL);
    }
  }
  
  return 0;
}
