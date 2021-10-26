#include <stdio.h>
#include <stdlib.h>
#include <openssl/md5.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdint.h>

#define HASHSIZE 8 // In bytes
#define SHIFT 3 // In bits

typedef uint64_t pwhash;

pwhash target_hash_function(char *pwd){
  unsigned char md5[MD5_DIGEST_LENGTH] = {0};
  MD5((const unsigned char *)pwd, strlen(pwd), md5);
  char hexamd5hash[2*HASHSIZE+1];
  hexamd5hash[2*HASHSIZE]='\0';
  for (int i=0; i < HASHSIZE; i++) {
    sprintf(hexamd5hash + 2*i, "%02x", md5[i]);
  }
  pwhash md5hash = strtoull(hexamd5hash, NULL, 16);
  pwhash myhash = (md5hash << SHIFT)|(md5hash >> (8*HASHSIZE - SHIFT));
  return myhash;
}
