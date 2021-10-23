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

void get_couples(char* word, char* x0, char* xL)
{
    if (word == NULL)
        generate_pwd(word);
    x0 = word;
    for(int cpt = 1; cpt <= L; cpt++)
    {
        reduction_function(target_hash_function(word), cpt-1, word);
    }
    xL = word;
}

int set_rainbow(FILE* f, FILE* refFile) //is given needed ?? Or determine if file empty ? 
{
    char couples[2][M];
    char initialWord[M];
    for (int nbChains = 1; nbChains <= N; nbChains++)
    {
        if (refFile == NULL)
            get_couples(NULL, couples[0], couples[1]);
        else
        {
            if (fgets(initialWord, M, refFile) == 0)
            {
                printf("Not enough word given in file\n");
                return 0;
            }

            get_couples(initialWord, couples[0], couples[1]);
        }
        fprintf(f, "%s %s%s", couples[0], couples[1], "\n");
    }

    free(couples);
    if(fclose(f))
    {   
        printf("Couldn't close the file\n");
        return 0;
    } 

    return 1;
}

int main(int argc, char* argv[])
{
    if (argc < R+1)
    {
        printf("You gave %i files to write. Give at least %i files in argument\n", argc-1, R);
        return -1;
    }

    FILE* file = NULL; FILE* refFile = NULL; 
    if (argc == R + 2)
        refFile = fopen(argv[R+1], "r");

    for (int nthFile=1; nthFile<=R; nthFile++)
    {
        file = fopen(argv[nthFile], "w");

        if(!set_rainbow(file, refFile))
            return -1;
    }
}