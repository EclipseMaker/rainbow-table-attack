
#include <assert.h>
#include <math.h>
#include <openssl/md5.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "hash.h"

#define M 6
#define NB_PASS_MAX 308915776
#define R 10
#define N 100000
#define L 1000
#define BASE 26
#define MAX_BUFFER 26
#define M_H 500000

typedef uint64_t pwhash;


struct cellule 
{
  char *nom;
  struct cellule *suivant;
};

typedef struct cellule cellule_t;

struct liste 
{
  cellule_t *tete;
};

typedef struct liste liste_t;

void initialisation(liste_t *Liste) { Liste->tete = NULL; }

int puissance(int x, int y) 
{
  // Déclaration des variables
  int compteur, resultat;

  compteur = 0;
  resultat = 1;

  while (compteur < y) 
  {
    resultat = resultat * x;
    compteur++;
  }

  return resultat;
}

int transf(char *chaine) {
  int lettre, nombre;
  nombre = 0;

  for (int i = 0; i < strlen(chaine); i++) 
  {
    lettre = chaine[i];
    nombre = nombre + lettre * puissance(26, i);
  }
  return nombre;
}

void ajouter(liste_t *Liste, char *info) 
{
  cellule_t *cel;
  cel = (cellule_t *)malloc(sizeof(cellule_t));
  cel->nom = (char *)malloc(sizeof(int));
  strcpy(cel->nom, info);
  if (Liste->tete == NULL)
  {
    cel->suivant = Liste->tete;
    Liste->tete = cel;
  } 
  else 
  {
    cellule_t *parcours = Liste->tete;
    while (parcours->suivant != NULL) 
      parcours = parcours->suivant;
    cel->suivant = parcours->suivant;
    parcours->suivant = cel;
  }
}

void free_liste(liste_t *Liste) {
  cellule_t *cel = Liste->tete;
  cellule_t *prec = Liste->tete;
  if (Liste != NULL) {
    if (Liste->tete != NULL) 
    {
      while (cel != NULL) 
      {
        cel = cel->suivant;
        free(prec);
        prec = cel;
      }
    }
  }
}

void free_table(liste_t **Liste) 
{
  for (int i = 0; i < M_H; i++)
    free_liste(Liste[i]);
}

bool detecter(liste_t *Liste, char *mot) 
{
  cellule_t *cel = Liste->tete;
  while (cel != NULL) 
  {
    if (strcmp(cel->nom, mot) == 0)
      return true;
    cel = cel->suivant;
  }
  return false;
}

void init_tableau(char *tab) 
{
  if (tab == NULL) {
    printf("probleme malloc\n");
    exit(-1);
  }
  for (int i = 0; i < M; i++)
    tab[i] = 0;
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
    tab[j] += 'a';
}

void reduction_function(uint64_t hash, int columnNumber, char *tab) 
{
  int modulo_hash = (hash % (NB_PASS_MAX)) + columnNumber;
  init_tableau(tab);
  change_base_function(modulo_hash, tab);
}

void generate_pwd(char *pwd) 
{
  for (int i = 0; i < M; i++)
    pwd[i] = rand() % 26 + 'a';
}

bool insert_data(char *info, liste_t **l, int *collision) 
{
    int nbr = transf(info) % M_H;
    if (nbr < 0) 
        nbr = nbr + M_H;

    if (detecter(l[nbr], info)) 
    {
        *(collision) = *(collision) + 1;
        return false;
    } 
    ajouter(l[nbr], info);
    return true;
}

void get_couples(char *word, char *x0, char *xL, int *collision, liste_t **tab1) 
{
  if (word == NULL) 
  {
    word = (char *)malloc(sizeof(char) * M);
    generate_pwd(word);
    while (!insert_data(word, tab1, collision))
      generate_pwd(word);
  }

  x0[M] = '\0';
  strcpy(x0, word);
  for (int cpt = 1; cpt <= L; cpt++) 
    reduction_function(target_hash_function(word), cpt - 1, word);

  xL[M] = '\0';
  strcpy(xL, word);
  //free(word);    ne pas oublier de free
}

void generate_chain(char *pwd) {
  uint64_t hash;
  for (int i = 0; i < L; i++) {
    hash = target_hash_function(pwd);
    reduction_function(hash, i, pwd);
  }
}

void generate_table(void) {
  char *pwd = (char *)malloc(sizeof(char) * M);

  if (pwd == NULL) {
    printf("probleme de malloc\n");
    exit(0);
  }
  for (int i = 0; i < N; i++) {
    generate_pwd(pwd);
    printf(" PWD(%d,0 : %s)  ||  ", i, pwd);
    generate_chain(pwd);
    printf("PWD (%d,L => %s)\n", i, pwd);
  }
}

int set_rainbow(FILE *f, FILE *refFile, liste_t **tab1, int *collision)
{
  char couples[2][M + 1];
  char initialWord[M + 1];
  liste_t *tab2[M_H];
  for (int i = 0; i < M_H; i++) 
  {
    liste_t *l;
    l = (liste_t *)malloc(sizeof(liste_t));
    initialisation(l);
    tab2[i] = l;
  }

  for (int nbChains = 1; nbChains <= N; nbChains++) 
  {
    if (refFile == NULL) {
      get_couples(NULL, couples[0], couples[1], collision, tab1);
      while (!insert_data(couples[1], tab2, collision))
        get_couples(NULL, couples[0], couples[1], collision, tab1);
    } 
    else 
    {
      if (fgets(initialWord, M + 1, refFile) == NULL) 
      {
        printf("Not enough word given in file\n");
        return 0;
      }
      if (fgetc(refFile) != '\n') 
      {
        fprintf(stderr, "word length error\n");
        return 0;
      }
      if (!feof(f)) 
      {
        get_couples(initialWord, couples[0], couples[1], collision, tab1);
        while (!insert_data(couples[1], tab2, collision))
          get_couples(NULL, couples[0], couples[1], collision, tab1);
      }
    }
    fprintf(f, "%s %s\n", couples[0], couples[1]);
  }

  free_table(tab2);
  if (fclose(f) == EOF) 
  {
    printf("Couldn't close the file\n");
    return 0;
  }

  return 1;
}