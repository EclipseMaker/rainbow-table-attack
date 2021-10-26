#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "time.h"


void creer_jeu_donnes(void){

  srand(time(NULL));
  char* tab=malloc(sizeof(char)*6);
  for(int i=0; i< 2; i++){
        generate_pwd(tab);
        generate_chain_with_file(tab);

  }

}

int main(int argc, char* argv[])
{

  creer_jeu_donnes();
  char** fileNames;
  FILE* fileToCrack;
  FILE* pwdFound;
  char* ligne;
  char* passXL;
  char *hashToCrack;
  hashToCrack = (char*)malloc(sizeof(char) * 21);


  if (argc!= R+3){ ///R le nombre de fichier generer par la fonction create tables
       fprintf(stderr,"wrong number of file\n");
       return -1;
  }

  fileToCrack = fopen(argv[R+1], "r");
  pwdFound = fopen("pwdFound.txt", "w");

  if(fileToCrack == NULL || pwdFound == NULL){
      printf("probleme douverture de fichier\n");
      exit(-1);
  }

  fileNames =(char**) malloc(sizeof(char*)*R);
  if(fileNames == NULL){
    printf("prob de malloc");
  }

   for(int i=1; i < argc - 2 ; i++){
     fileNames[i-1]=strdup(argv[i]);
  }

   rainbow_attack(fileNames, fileToCrack, pwdFound);
  // for (int i = 0; i < R; i++){
  //   printf("%s\n", fileNames[i]);
  // }

/*   for(int i=1; i < (argc-2); i++){
     fclose(fileNames[i-1]);
  }*/
}