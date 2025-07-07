#include "htable.h"
#include <stdio.h>
#include <assert.h>

DEFINE_HTABLE(int , int , int_int_table) ;

// Test initializing and freeing
void test_init_free() {
  printf("Testing init and free ... \n");
  int_int_table *table = ht_init(INITIAL_CAPACITY);
  assert(table != NULL);
  assert(table->size == 0);
  assert(table->capacity == INITIAL_CAPACITY);
  assert(table->entries != NULL) ;
  for (int i = 0; i < table->capacity; i++) {
    assert(table->entries[i].in_use == 0 ) ; 
  }
  ht_free(table);
  printf("Test init and free passed.\n") ; 
  
}

// Test insertion and retrieval
void test_insert_get() {
  printf("Testing insert and get ...\n");
  int_int_table *table = ht_init(INITIAL_CAPACITY);
  assert(table != NULL);

  // Insert key-value pairs
  assert(ht_insert(table, 1, 100) == 1);
  assert(table->size == 1);
  assert(ht_insert(table, 2, 200) == 1);
  assert(table->size == 2);

  // Retrieve values
  int *value = ht_get(table, 1);
  assert(value != NULL && *value == 100);
  value = ht_get(table, 2);
  assert(value != NULL && *value == 200);
  value = ht_get(table, 3); // Non-existing key
  assert(value == NULL);

  // Update existing key
  assert(ht_insert(table, 1, 150) == 1);
  assert(table->size == 2); // size should not increase
  value = ht_get(table, 1);
  assert(value != NULL && *value == 150);

  ht_free(table);
  printf("Test insert and get passed \n") ; 
}

// Test removal
void test_remove() {
  printf("Testing remove ...\n");
  int_int_table *table = ht_init(INITIAL_CAPACITY);
  assert(table != NULL);

  // Inserting some entries
  assert(ht_insert(table, 1, 100) == 1);
  assert(ht_insert(table, 2, 200) == 1);
  assert(table->size == 2 ) ; 

  // Remove existing key
  assert(ht_remove(table, 1) == 1);
  assert(ht_get(table, 1) == NULL);

  // Remove non_existing key
  assert(ht_remove(table, 3) == 0);
  assert(table->size ==1 ) ;

  // Remove last-key
  assert(ht_remove(table, 2) == 1);
  assert(table->size == 0);
  assert(ht_get(table, 2) == NULL);

  ht_free(table);
  printf("Test remove passed.\n") ; 
}

// Test resising (load factor > 70%)
void test_resize() {
  printf("Testing resize ..\n");
  int_int_table *table = ht_init(INITIAL_CAPACITY);
  assert(table != NULL);

  // Insert 6 enries ( * 0.7 = 5.6 , so 6 entries should
  // trigger resize
  for (int i = 0; i < 7; i++) {
    assert(ht_insert(table, i , i * 100) == 1 ) ; 
  }
  assert(table->size == 7);
  assert(table->capacity == 16); // Should have doubled

  // Verify all entries are still accessible
  for (int i = 0; i < 6; i++) {
    int *value = ht_get(table, i);
    assert(value != NULL && *value == i *100) ; 
  }

  ht_free(table);
  printf("Test resize passed.\n") ; 
}


// Test edge cases
void test_edge_cases() {
  printf("Testing edge cases ...\n");

  // NULL table
  assert(ht_insert(NULL, 1 , 100 ) == 0 ) ;
  assert(ht_get(NULL, 1) == NULL) ; 
  assert(ht_remove(NULL, 1) == 0 ) ; 
  ht_free(NULL) ; // shoud not crasch

  // Empty table
  int_int_table *table = ht_init(INITIAL_CAPACITY);
  assert(table != NULL);
  assert(ht_get(table, 1) == NULL);
  assert(ht_remove(table, 1) == 0);

  // Insert with collisions (keys that hash to the
  // same index)
  // Assuming ht_hash(100) and ht_hash(108) collide
  // (depends on capacity =8)
  assert(ht_insert(table, 100, 1000) == 1);
  assert(ht_insert(table, 108, 2000) == 1); // Likely collide with 100
  assert(table->size == 2);

  int *value = ht_get(table, 100);
  assert(value != NULL && *value == 1000) ;
  value = ht_get(table, 108);
  assert(value != NULL && *value == 2000) ;

  ht_free(table);
  printf("Test edge cases passed.\n") ; 
}

// Test maximum capacity limit
void test_max_capacity() {
  printf("Testing max capacity...\n");
  int_int_table *table = ht_init(MAX_CAPACITY / 2);
  assert(table != NULL);

  // Fill table to trigger resize up to MAX_CAPACITY
  int threshold = (MAX_CAPACITY * 7) / 10;
  for (int i = 0; i < threshold; i++) {
    assert(ht_insert(table, i , i * 100) == 1 )  ; 
  }
  assert(table->capacity == MAX_CAPACITY ) ; 

  // Try to insert one more to trigger resize
  // (should fail due to MAX_CAPCAITY )
  assert(ht_insert(table, threshold, threshold * 100) == 1);
  assert(table->capacity == MAX_CAPACITY); // Should not grow

  // Verify entries
  for (int i = 0; i < threshold; i++) {
    int *value = ht_get(table, i);
    assert(value != NULL && *value == i * 100 ); 
  }
  ht_free(table);
  printf("Test max capacity passed.\n") ; 
}
int main_two() {
  int_int_table *table = ht_init(INITIAL_CAPACITY);
  for (int i = 0; i < 7; i++) {
    ht_insert(table, i , i * 10 ) ; 
  }
  for (int i = 0; i < table->capacity; i++) {
    printf(" [ %d ]  %d   %5d \n", i , table->entries[i].key , table->entries[i].value ) ; 
  }
  ht_free(table) ; 
  return 0; 
}
int main() {
  printf("Running hash table tests...\n");
  test_init_free();
  test_insert_get();
  test_remove();
  test_resize();
  test_edge_cases();
  test_max_capacity();
  printf("All tests passed.\n");
  return 0 ; 
}
