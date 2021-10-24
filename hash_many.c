#include <stdlib.h>
#include <openssl/md5.h>
#include <string.h>
#include <stdio.h>
#include "hash.h"


int writeHash(FILE* pwdList, FILE* destFile)
{
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int cpt = 0;
    while ((read = getline(&line, &len, pwdList)) != -1)
    {
        fprintf(pwdList, "%lu\n", target_hash_function(line));
        cpt++;
    }

    return cpt;
}

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "Please give at least a password file and a destination file for hashes");
        return -1;
    }

    FILE* pwdList = fopen(argv[1], "r");
    if (pwdList == NULL)
    {
        fprintf(stderr,"cannot open file %s\n",argv[1]);
        return -1;
    }

    FILE* destFile = fopen(argv[2], "w");
    if (destFile == NULL)
    {
        fprintf(stderr,"cannot open file %s\n",argv[1]);
        return -1; 
    }

    printf("%i hashes been written in the following file : %i \n", writeHash(pwdList, destFile), argv[2]);
    return 0;
}