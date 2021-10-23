#include <stdlib.h>
#include <openssl/md5.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#define HASHSIZE 8 // In bytes
#define SHIFT 3    // In bits
typedef uint64_t pwhash;
#define M 6
#define NB_PASS_MAX 308915776
#define R 10
#define N 100000
#define L 10
#define BASE 26
#define HASHSIZE 8 // In bytes
#define SHIFT 3    // In bits
#define MAX_BUFFER 26

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

void init_tableau(char *tab)
{
  if (tab == NULL)
  {
    printf("probleme malloc\n");
    exit(-1);
  }
  for (int i = 0; i < M; i++)
  {
    tab[i] = 0;
  }
}

void change_base_function(int number, char *tab)
{

  int quotient = 0;
  int remainder = 0;
  int i = M - 1;

  while ((number / BASE) != 0)
  {
    tab[i] = (number % BASE);
    number = number / BASE;
    i--;
  }

  tab[i] = (number % BASE);
  for (int j = 0; j < M; j++)
  {
    tab[j] += 'a';
    // printf("tableau %c\n",tab[i]);
  }
}

void reduction_function(uint64_t hash, int columnNumber, char *tab)
{

  int modulo_hash = (hash % (NB_PASS_MAX)) + columnNumber;
  // char *tab= malloc(sizeof(char)*M);
  init_tableau(tab);
  change_base_function(modulo_hash, tab);
  // printf("Mon mots de passe est : %s\n",tab);
}


void generate_pwd(char *pwd)
{

  for (int i = 0; i < M; i++)
  {
    pwd[i] = rand() % 26 + 'a';
  }
}

void get_couples(char* word, char* x0, char* xL)
{
    if (word == NULL){
        word=(char*)malloc(sizeof(char)*M);
        generate_pwd(word);
}
    x0[M]='\0';
    strcpy(x0,word);
    for(int cpt = 1; cpt <= L; cpt++)
    {
        reduction_function(target_hash_function(word), cpt-1, word);
    }
    xL[M]='\0';
    strcpy(xL,word);
//    free(word);    ne pas oublier de free
 }
 void generate_chain(char *pwd){
   uint64_t hash;
   for (int i = 0; i < L; i++)
   {
     hash = target_hash_function(pwd);
     reduction_function(hash, i, pwd);
   }
 }



void generate_table(void){
    char *pwd=(char*)malloc(sizeof(char)*M);

    srand(time(NULL));
    if(pwd == NULL){
        printf("probleme de malloc\n");
        exit(0);
    }
    for(int i=0; i<N ; i++){
        generate_pwd(pwd);
        // passeword exist ? chkeck it with function verify : True->existe  False:doesn't exist
        printf(" PWD(%d,0 : %s)  ||  ", i, pwd);
        generate_chain(pwd);
        // passeword exist ? chkeck it with function verify : True->existe  False:doesn't exist
        printf("PWD (%d,L => %s)\n", i, pwd);

    }

}


int set_rainbow(FILE* f, FILE* refFile) //is given needed ?? Or determine if file empty ?
{
    char couples[2][M+1];
    char initialWord[M];
    for (int nbChains = 1; nbChains <= N; nbChains++)
    {
        if (refFile == NULL)
   {

            get_couples(NULL,couples[0], couples[1]);
 }
        else
        {
            if (fgets(initialWord, M, refFile) == 0)
            {

                printf("Not enough word given in file\n");
                return 0;
            }

            get_couples(initialWord, couples[0], couples[1]);

        }
        fprintf(f, "%s %s\n", couples[0], couples[1]);
    }


    //free(couples)    renvoie une erreur , faut faire une fonction qui free un tableau case par case

    if(fclose(f)==EOF)
    {
        printf("Couldn't close the file\n");
        return 0;
    }

    return 1;
}




