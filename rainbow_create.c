#include <stdio.h>
#include <stdlib.h>
#include "rainbow_create.h"

int main(int argc, char *argv[])
{
    FILE *file = NULL;
    FILE *refFile = NULL;

    if (argc < R + 1)
    {
        printf("You gave %i files to write. Give at least %i files in argument\n", argc - 1, R);
        return -1;
    }
    if (argc == R + 2)
    {
        refFile = fopen(argv[R + 1], "r");
        if (refFile == NULL)
        {
            fprintf(stderr, "cannot open file %s\n", argv[R + 1]);
            return -1;
        }
    }

    //On va creer deux tableaux, Tab1 qui contient tout les PWD (x,0)  et Tab2 qui contient les PWD (x,L) déjà obtenu lors
    //de la construction de nos R tables.

    liste_t *tab1[M_H];
    // liste_t *tab2[M_H];
    int collision = 0;
    for (int i = 0; i < M_H; i++)
    {
        liste_t *l;
        l = (liste_t *)malloc(sizeof(liste_t));
        initialisation(l);
        tab1[i] = l;
    }

    srand(time(NULL));
    for (int nthFile = 1; nthFile <= R; nthFile++)
    {
        file = fopen(argv[nthFile], "w");
        if (!set_rainbow(file, refFile, tab1, &collision))
            return -1;
        printf("Table n°%d has been generated\n", nthFile);
    }
    printf("%i tables has been generated with %d collisions\n", R, collision);
    free_table(tab1);
    return 0;
}
