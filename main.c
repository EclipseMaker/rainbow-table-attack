#include "hash.h"

void test_generation_chain(void){

     char* tab=malloc(sizeof(char)*6);
     init_tableau(tab);
     tab[0]='w';
     tab[1]='y';
     tab[2]='u';
     tab[3]='u';
     tab[4]='r';
     tab[5]='o';

     generate_chain(tab) ;
     printf("%s\n", tab);

      printf("wyuuro ");
      uint64_t  hash;
      hash=target_hash_function("wyuuro ");
       reduction_function(hash,0,tab);
      printf("%s\n", tab);

}

/*int main(int argc, char* argv[])
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
}*/

int main (int argc, char* argv[])
{
        //generate_table();
        test_generation_chain();
}
