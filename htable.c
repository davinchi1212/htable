#include "htable.h"
#include <stdint.h>

// hash function for string value ( const char* )
uint64_t ht_hash(const char *key) {
  uint64_t hash =  OFFSET_HASH ;
  const unsigned char *p = (const unsigned char *)key;
  while(*p) {
    hash ^= (uint64_t)(*p) ;
    hash *= FNV_PRIME ;
    p++ ;
  }
  return hash;
}
// hash function for generic const void* + size value
//uint64_t mytable_hash(const void* data, size_t size ) {
//  const unsigned char *p = (const unsigned char *)data;
//  uint64_t hash =  OFFSET_HASH ;
//  for (size_t i = 0; i < size; i++) {
//    hash ^= p[i];
//    hash *= FNV_PRIME ;
//    p++ ;
//  }
//  return hash;
//}
