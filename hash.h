#include <stdlib.h>
#include <openssl/md5.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdint.h>
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
  for (int i = 0; i < M; i++)
  {
    tab[i] += 'a';
    // printf("tableau %c\n",tab[i]);
  }
}

void reduction_function(uint64_t hash, int columnNumber, char *tab)
{

  int modulo_hash = (hash % (NB_PASS_MAX)) + columnNumber;
  // char *tab= malloc(sizeof(char)*M);
  //init_tableau(tab);
  change_base_function(modulo_hash, tab);
  // printf("Mon mots de passe est : %s\n",tab);
}

void generate_pwd(char *pwd)
{

  // seed
  srand(time(NULL));
  for (int i = 0; i < M; i++)
  {
    pwd[i] = rand() % 26 + 'a';
  }
}

void generate_chain(char *pwd)
{
  uint64_t hash;
  for (int i = 0; i < L; i++)
  {
    printf("tema le mdp : %s ", pwd);
    hash = target_hash_function(pwd);
    reduction_function(hash, i, pwd);
  }
}

void generate_table(void){
    char *pwd;
    pwd=(char*)malloc(sizeof(pwd)*M);
    if(pwd == NULL){
        printf("probleme de malloc\n");
        exit(0);
    }
    for(int i=0; i<N ; i++){
        generate_pwd(pwd);
        printf(" pwd  %d,0 : %s", i, pwd);
        generate_chain(pwd);

        printf("pwd%d,L => %s\n", i, pwd);

    }

}
