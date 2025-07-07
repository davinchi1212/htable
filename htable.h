#ifndef HTABLE_H_
#define HTABLE_H_
#include <stdlib.h>
#include <stdint.h>
#include <string.h> 
#include <assert.h> 


#define OFFSET_HASH 14695981039346656037ULL
#define FNV_PRIME   1099511628211ULL
#define MAX_CAPACITY     1048576 // (1<<20 )
#define INITIAL_CAPACITY 8
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

#define DEFINE_HTABLE(KT, KV, NAME)                                            \
  typedef struct ht_entry {                                                    \
    KT key;                                                                    \
    KV value;                                                                  \
    int in_use;                                                                \
  } ht_entry;                                                                  \
                                                                               \
  typedef struct NAME {                                                        \
    int size;                                                                  \
    int capacity;                                                              \
    ht_entry *entries;                                                         \
  } NAME;                                                                      \
                                                                               \
  NAME *ht_init(int initial_capacity) {                                        \
    assert(((initial_capacity) & (initial_capacity - 1)) == 0);                \
    NAME *table = malloc(sizeof(NAME));                                        \
    if (!table)                                                                \
      return NULL;                                                             \
    table->size = 0;                                                           \
    table->capacity = initial_capacity;                                        \
    table->entries = calloc(table->capacity, sizeof(ht_entry));                \
    if (!table->entries) {                                                     \
      free(table);                                                             \
      return NULL;                                                             \
    }                                                                          \
    return table;                                                              \
  }                                                                            \
                                                                               \
  void ht_free(NAME *table) {                                                  \
    if (table) {                                                               \
      free(table->entries);                                                    \
      free(table);                                                             \
    }                                                                          \
  }                                                                            \
                                                                               \
  static inline uint64_t ht_hash(KT key)				\
  {									\
    size_t len = sizeof(KT)	; 					\
    uint64_t hash = OFFSET_HASH ;					\
    const uint8_t* p = (const uint8_t*)&(key );				\
    for ( size_t i =0 ; i < len ; i ++ ) {				\
      hash ^= (uint64_t)(p[i]) ;					\
      hash *= FNV_PRIME ;						\
    }									\
    return hash ;							\
  }									\
  static inline int grow_capacity(int capacity) {			\
    assert(capacity <= MAX_CAPACITY && (capacity << 1) <= MAX_CAPACITY); \
    return capacity << 1;						\
  }									\
  									\
  static inline void copy_table(NAME **table) {				\
    int old_capacity = (*table)->capacity;				\
    ht_entry *old_entries = (*table)->entries;				\
    (*table)->capacity = grow_capacity(old_capacity);			\
    ht_entry *new_entries =						\
      (ht_entry *)calloc((*table)->capacity, sizeof(ht_entry));		\
    if (!new_entries) {							\
      (*table)->capacity = old_capacity;				\
      (*table)->entries = old_entries;					\
      return;								\
    }									\
    									\
    (*table)->size = 0;							\
    for (int i = 0; i < old_capacity; i++) {				\
      ht_entry entry = old_entries[i];					\
      if (entry.in_use) {						\
        uint64_t idx = (ht_hash(entry.key)) & ((*table)->capacity - 1);	\
        while (new_entries[idx].in_use) {				\
          idx = (idx + 1) & ((*table)->capacity - 1);			\
        }								\
        new_entries[idx].key = entry.key;				\
        new_entries[idx].value = entry.value;				\
        new_entries[idx].in_use = 1;					\
        (*table)->size++;						\
      }									\
    }									\
    free(old_entries);							\
    (*table)->entries = new_entries;					\
  }									\
  									\
  static inline int ht_insert(NAME *table, KT key, KV value) {		\
    if (!table || !table->entries)					\
      return 0;								\
    if (((table->size )  * 10) >= (table->capacity * 7)){		\
      copy_table(&table);						\
      if (!table->entries) return 0;					\
    }									\
                                                                               \
    uint64_t idx = ht_hash(key) & (table->capacity - 1);                       \
    while (table->entries[idx].in_use) {                                       \
      if (memcmp(&table->entries[idx].key, &key, sizeof(KT)) == 0) {           \
        table->entries[idx].value = value;                                     \
        return 1;                                                              \
      }                                                                        \
      idx = (idx + 1) & (table->capacity - 1);                                 \
    }                                                                          \
    table->entries[idx].key = key;                                             \
    table->entries[idx].value = value;                                         \
    table->entries[idx].in_use = 1;                                            \
    table->size++;                                                             \
    return 1;                                                                  \
  }                                                                            \
                                                                               \
  static inline KV *ht_get(NAME *table, KT key) {                              \
    if (!table || table->size == 0)                                            \
      return NULL;                                                             \
    uint64_t idx = ht_hash(key) & (table->capacity - 1);                       \
    while (table->entries[idx].in_use) {                                       \
      if (memcmp(&table->entries[idx].key, &key, sizeof(KT)) == 0)             \
        return &table->entries[idx].value;                                     \
      idx = (idx + 1) & (table->capacity - 1);                                 \
    }                                                                          \
    return NULL;                                                               \
  }                                                                            \
                                                                               \
  static inline int ht_remove(NAME *table, KT key) {                           \
    if (!table || table->size == 0)                                            \
      return 0;                                                                \
    uint64_t idx = ht_hash(key) & (table->capacity - 1);                       \
    while (table->entries[idx].in_use)					\
    {									\
      if (memcmp(&table->entries[idx].key, &key , sizeof(KT) ) == 0 ) {	\
	table->entries[idx].in_use = 0 ;				\
	table->size -- ;						\
	return 1 ;							\
      }									\
      idx = (idx +  1) & (table->capacity - 1) ;			\
    }									\
    return 0 ;								\
  }
  

#endif
