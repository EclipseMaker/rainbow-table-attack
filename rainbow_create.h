#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "./hash.h"
#define M 6
#define R 10
#define N 100000
#define L 1000

char* reduction_with_hash(int cpt)
{
    char * word = "arvind";
    word += cpt;
    return word;
    //TO REPLACE
}

char* random_word(int letter)
{
    return "arvind";
    //TO REPLACE
}

char[M][2] get_chain(char[M] word)
{
    char couples[2][M] = NULL;
    if (word == NULL)
        word = random_word(M);
    couples[0] = word;
    for(int cpt = 1; cpt <= L; cpt++)
        word =  reduction_with_hash(target_hash_function(word));
    couples[1] = word;
    return couples;
}

int set_rainbow(FILE* f, bool isGiven) //is given needed ?? Or determine if file empty ? 
{
    char couples[2][M] = NULL;
    char initialWord[M] = NULL;
    for (int nbChains = 1; nbChains <= N; nbChains++)
    {
        if (!isGiven)
            couples = get_chain(NULL);
        else
        {
            if (fgets(initialWord, M, f) == 0)
            {
                print("Not enough word given in file\n");
                return 0;
            }

            couples = get_chain(initialWord);
        }
        fprintf(f, "%s %s%s", couples[0], couples[1], "\n");
    }

    free(couples);
    if (fclose(file))
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

    FILE* f = NULL;
    for (int nthFile=1; nthFile<=R; nthFile++)
    {
        f = fopen(argv[i], "r");
        if (!f)
        {
            perror(argv[i]);
            return EXIT_FAILURE;
        }

        if(!set_rainbow(f, false))
            return -1;
    }

    for (int nthFile = R + 1; nthFile < argc; nthFile++)
    {
        f = fopen(argv[i], "w");
        if (!f)
        {
            perror(argv[i]);
            return EXIT_FAILURE;
        }

        if(!set_rainbow(f, false))
            return -1;
    }
}

//${workspaceFolder}/**