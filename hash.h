
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <openssl/md5.h>
#include <time.h>

#define HASHSIZE 8 // In bytes
#define SHIFT 3    // In bits
#define M 6
#define NB_PASS_MAX 308915776
#define R 10
#define N 100000
#define L 1000
#define BASE 26
#define HASHSIZE 8 // In bytes
#define SHIFT 3    // In bits
#define MAX_BUFFER 26

typedef uint64_t pwhash;


pwhash target_hash_function(char *pwd)
{
  unsigned char md5[MD5_DIGEST_LENGTH] = {0};
  MD5((const unsigned char *)pwd, strlen(pwd), md5);
  char hexamd5hash[2 * HASHSIZE + 1];
  hexamd5hash[2 * HASHSIZE] = '\0';
  for (int i = 0; i < HASHSIZE; i++)
  {
    sprintf(hexamd5hash + 2 * i, "%02x", md5[i]);
  }
  pwhash md5hash = strtoull(hexamd5hash, NULL, 16);
  pwhash myhash = (md5hash << SHIFT) | (md5hash >> (8 * HASHSIZE - SHIFT));
  return myhash;
}


void change_base_function(int number, char *tab)
{

  int quotient = 0;
  int remainder = 0;
  int i = M - 1;

  while ((number / BASE) != 0){
    tab[i] = (number % BASE);
    number = number / BASE;
    i--;
  }

  tab[i] = (number % BASE);
  for (int j = 0; j < M; j++)
  {
    tab[j] += 'a';
  }
}

void init_tableau(char *tab)
{
  if (tab == NULL){
    printf("probleme malloc\n");
    exit(-1);
  }
  for (int i = 0; i < M; i++){
    tab[i] = 0;
  }
}

void reduction_function(uint64_t hash, int columnNumber, char *tab)
{
  int modulo_hash = (hash % (NB_PASS_MAX)) + columnNumber;
  init_tableau(tab);
  tab[M] = '\0';
  change_base_function(modulo_hash, tab);
}


char* research_pwd(char* pwd, FILE* fileName){

  char* passXL=(char*)malloc(sizeof(char)*M);
 char* passX0=(char*)malloc(sizeof(char)*M);
  int numLigne = 0;
  int j=0;


  fscanf(fileName,"%s %s", passX0, passXL);
  ///// problème possible
 // printf("Lik %s %s\n", passX0, passXL);

  while(!feof(fileName)){
    if(strcmp(passXL, pwd) == 0){
      printf("le pass XL trouve est : %s", pwd);
      return passX0;
    }
    fscanf(fileName,"%s %s", passX0, passXL);
  }

  return NULL;

}
void write_in_pwd_file(char* pwd,  FILE* foundPwd){

  printf("le mot de passe que je vais ecrire %s \n", pwd);
  fprintf(foundPwd, "%s ", pwd);

}

char* find_antecedent(char* passX0, char* hashToCrack, int n){
    uint64_t hash;
    char* tab;
    tab = (char*)malloc(sizeof(char)*255);
    if(tab == NULL){
      printf("pb de malloc du tableau init tableau\n");
      exit(-1);
    }
    init_tableau(tab);

    printf("Le pass XO utilisé %s\n", passX0);
    hash = target_hash_function(passX0);

    if( hash == strtoul(hashToCrack, NULL, 10)){
      printf("le hash trouve : %lu et le hash a attaquer %s\n", hash, hashToCrack);
      return passX0;
    }

    for(int i=0; i< n; i++){

      reduction_function(hash, i, tab);
      hash = target_hash_function(tab);

      if( hash == strtoul(hashToCrack, NULL, 10)){
        printf("le hash trouve : %lu et le hash a attaquer %s\n", hash, hashToCrack);
        return tab;
      }
    }

    return NULL;
}

void generate_pwd(char *pwd)
{
  for (int i = 0; i < M; i++)
  {
    pwd[i] = rand() % 26 + 'a';
  }
}

void generate_chain(char *pwd){
   uint64_t hash;
   for (int i = 0; i < L; i++)
   {
     hash = target_hash_function(pwd);
     reduction_function(hash, i, pwd);

   }
 }

void generate_chain_with_file(char *pwd){
  FILE* fe;
  uint64_t hash;
  fe = fopen("test.txt", "aw");
  if(fe == NULL){
    printf("probleme d'ouverture de fichier");
    exit(-1);
  }
  fprintf(fe, "%s ", pwd);
  for (int i = 0; i < L; i++)
  {
    hash = target_hash_function(pwd);
    fprintf(fe, "%lu ", hash);
    reduction_function(hash, i, pwd);
    fprintf(fe, "%s ",pwd);
  }
  fprintf(fe,"\n\n");
  fclose(fe);
}


void rainbow_attack(char** fileNames, FILE* fileToCrack, FILE* fileFoundPwd ){

  char* hashToCrack = NULL;
  char *reducedPasswd;
  char *antecedent=NULL;
  char *passX0Trouve;

///////////////////////////////////////////////
  int limite;
  int j = L-1,  number = 0, compteur = 0;
  size_t len = 0;
  ssize_t read;
  FILE* fe;

  for(int i=0; i < R ; i++){
    printf("%s\n", fileNames[i]);

  }

  while( (read = getline(&hashToCrack, &len, fileToCrack)) != -1)
  {

    printf("le hash à craquer %s \n", hashToCrack);
    reducedPasswd = (char*)malloc(sizeof(char)*M);
    if(reducedPasswd == NULL)
    {
      printf("pb de malloc du tableau init tableau\n");
      exit(-1);
    }

    init_tableau(reducedPasswd);
    limite = 0;
    passX0Trouve = NULL;

    while ((antecedent == NULL) && (limite <= (L-1)))
    {
      reduction_function(strtoul(hashToCrack, NULL, 10), (L-1) - limite, reducedPasswd);
      for(j = ((L-1) - limite) + 1 ; j <= L-1; j++)
      {
          reduction_function(target_hash_function(reducedPasswd),j, reducedPasswd);
          reducedPasswd[M]='\0';

      }
      limite++;

      //printf("le mot de passe a chercher parmis les pass xl %s\n", reducedPasswd);
      //printf("NOTRE N %d\n", (L-1) - limite);

      for(int i = 0; i < R; i++)
      {
        fe = fopen(fileNames[i], "r");
        if(fe == NULL){
          printf("pb d'ouverture de fichier\n");
          exit(-1);
        }
        passX0Trouve = research_pwd(reducedPasswd, fe);


        if(passX0Trouve != NULL)
        {
          printf("pass X0 : %s, pass Xl %s\n",passX0Trouve,reducedPasswd);
          antecedent = find_antecedent(passX0Trouve, hashToCrack, L - 1 - limite );
          if(antecedent !=NULL)
          {
            ///BIG PROBLEME ALGORITHMIQUE
             printf("Mon frérot ! j'ai trouver un antecedent dans le fichier %s   avec : pass X0 : %s, pass Xl %s\n",fileNames[i],passX0Trouve,reducedPasswd);

            printf("Hash : %s => Antecedent %s \n", hashToCrack, antecedent);
            write_in_pwd_file(antecedent, fileFoundPwd);
            break;
          }
         else if (antecedent == NULL)
      {
        printf("je n'ai pas trouver d'antecedent dans le fichier %s   avec : pass X0 : %s, pass Xl %s\n",fileNames[i],passX0Trouve,reducedPasswd);
       /* printf("VIDE\n");
        write_in_pwd_file("VIDE\n", fileFoundPwd);
        break;*/
      }
        }
        fclose(fe);

      }
    }
  }

}








