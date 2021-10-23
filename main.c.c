#include <stdio.h>
#include <stdlib.h>
#include <openssl/md5.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "hash.h"

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

    return 0;
}