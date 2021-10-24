#include "hash.h"
#include <openssl/md5.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int writeHash(FILE *pwdList, FILE *destFile) {
  char *tab = malloc(sizeof(char) * M);
  char c;
  int cpt = 1;
  while (true) {
    int i = 0;
    while (i <= M) {
      tab[i] = fgetc(pwdList);
      i++;
      if (feof(pwdList))
        return cpt;
    }
    fprintf(destFile, "%lu\n", target_hash_function(tab));
    cpt++;
  }

  return 0;
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(stderr, "Please give at least a password file and a destination "
                    "file for hashes");
    return -1;
  }

  FILE *pwdList = fopen(argv[1], "r");
  if (pwdList == NULL) {
    fprintf(stderr, "cannot open file %s\n", argv[1]);
    return -1;
  }

  FILE *destFile = fopen(argv[2], "w");
  if (destFile == NULL) {
    fprintf(stderr, "cannot open file %s\n", argv[1]);
    return -1;
  }
  printf("%d hashes been written in the following file : %s \n",
         writeHash(pwdList, destFile), argv[2]);
  return 0;
}
