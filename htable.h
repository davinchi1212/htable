#ifndef HTABLE_H_
#define HTABLE_H_
#include <stdint.h> 
#include <stdlib.h> 



#define OFFSET_HASH 14695981039346656037ULL
#define FNV_PRIME   1099511628211ULL
#define INITIAL_CAPACITY 8

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
    NAME *table = malloc(sizeof(NAME));                                        \
    table->size = 0;                                                           \
    table->capacity = initial_capacity;                                        \
    table->entries = calloc(table->capacity, sizeof(ht_entry));                \
    return table;                                                              \
  }                                                                            \
                                                                               \
  void ht_free(NAME *table) {                                                  \
    free(table->entries);                                                      \
    free(table);                                                               \
  }                                                                            \
                                                                               \
  uint64_t ht_hash(KT key);                                                    \
  static inline int grow_capacity(int capacity) { return capacity << 1; }      \
                                                                               \
  static inline NAME *copy_table(NAME *table) {                                \
    int old_capacity = table->capacity;                                        \
    ht_entry* old_entries = table->entries;				\
    ht_entry* new_entries = (ht_entry*)calloc(old_capacity << 2 , sizeof(ht_entry)) ; \
    table->capacity = old_capacity << 2 ;				\
    table->size = 0 ;							\
									\
									\
    for (int i = 0; i < old_capacity; ++i) {				\
      ht_entry entry = old_entries[i];					\
      if (entry.in_use) {						\
        uint64_t idx = (ht_hash(entry.key)) & (table->capacity - 1); \
	while(new_entries[i].in_use)					\
	{								\
	  idx = (idx + 1) & (table->capacity -1 ) ;			\
	}								\
	new_entries[idx].key = entry.key ;				\
	new_entries[idx].value = entry.value ;				\
	new_entries[idx].in_use = 1 ;					\
	table->size ++ ;						\
      }									\
    }									\
    free(old_entries) ;							\
    table->entries = new_entries ; 					\
    return table ;							\
  }									\
   static inline int ht_insert(NAME *table, KT key, KV value)		\
   {									\
     if ((table->size * 100) / table->capacity >= 70)			\
       table = copy_table(table) ;					\
									\
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
                                                                               \
  static inline int ht_remove(NAME *table, KT key)			\
  {									\
    if (table->size == 0 ) return  0 ;					\
    uint64_t idx  = ht_hash(key) & (table->capacity - 1) ;		\
    int i = 0 ;								\
    while(table->entries[idx].in_use) {					\
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
