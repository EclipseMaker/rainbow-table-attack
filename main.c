#include <stdio.h>
#include <stdlib.h>
#include <openssl/md5.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "hash.h"

int main (int argc, char* argv[])
{
//    printf("%"PRIu64 "\n", target_hash_function("arvind"));
   // int tab[6] = {0, 4, 8, 9, 6};
//   reduction_function(486486545,1);
   char* tab=malloc(sizeof(char)*6);
   init_tableau(tab);
   tab[0]='w';
   tab[1]='y';
   tab[2]='u';
   tab[3]='u';
   tab[4]='r';
   tab[5]='o';


//   generate_pwd(tab);
   printf("clès : %s - ", tab);
   free(tab);
   generate_chain(tab) ;
   printf("%s\n", tab);

    printf("wyuuro ");
    uint64_t  hash;
    hash=target_hash_function("wyuuro ");

    reduction_function(hash,0,tab);
    printf("%s\n", tab);
 //   printf("apres appel ");
}

