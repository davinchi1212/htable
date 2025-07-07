#include "htable.h"
#include <stdint.h>

// hash function for string value ( const char* )
//uint64_t ht_hash(int *key) {
//  uint64_t hash =  OFFSET_HASH ;
//  //const unsigned char *p = (const unsigned char *)key;
//  while(*p) {
//    hash ^= (uint64_t)(*p) ;
//    hash *= FNV_PRIME ;
//    p++ ;
//  }
//  return hash;
//}
// hash function for generic const void* + size value
uint64_t ht_hash(int key ) {
  // const unsigned char *p = (const unsigned char *)data;
  uint64_t hash =  OFFSET_HASH ;
  for (int i = 0; i < sizeof(int); i++) {
    hash ^= (key >> (i * 8) ) & 0xFF;
    hash *= FNV_PRIME ;
  }
  return hash;
}
