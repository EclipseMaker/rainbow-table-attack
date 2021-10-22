#include <stdio.h>
#include <stdlib.h>
#include <openssl/md5.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "hash.h"

int main (int argc, char* argv[])
{
    printf("%"PRIu64 "\n", target_hash_function("arvind"));
    printf("");
}