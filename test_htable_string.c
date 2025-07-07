#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "htable.h"

// Define a hash table with const char* keys and int values
DEFINE_HTABLE(const char*, int, str_int_table);

// Hash function for const char* keys (FNV-1a)
//uint64_t ht_hash(const char* key) {
//  uint64_t hash = OFFSET_HASH;
//  for (int i = 0; key[i] != '\0'; i++) {
//    hash ^= (unsigned char)key[i];
//    hash *= FNV_PRIME;
//  }
//  return hash;
//}

// Test initialization and freeing
void test_init_free() {
  printf("Testing init and free (string keys)...\n");
  str_int_table *table = ht_init(INITIAL_CAPACITY);
  assert(table != NULL);
  assert(table->size == 0);
  assert(table->capacity == INITIAL_CAPACITY);
  assert(table->entries != NULL);
  for (int i = 0; i < table->capacity; i++) {
    assert(table->entries[i].in_use == 0);
  }
  ht_free(table);
  printf("Test init and free passed.\n");
}

// Test insertion and retrieval
void test_insert_get() {
  printf("Testing insert and get (string keys)...\n");
  str_int_table *table = ht_init(INITIAL_CAPACITY);
  assert(table != NULL);

  // Insert key-value pairs
  assert(ht_insert(table, "one", 100) == 1);
  assert(table->size == 1);
  assert(ht_insert(table, "two", 200) == 1);
  assert(table->size == 2);

  // Retrieve values
  int *value = ht_get(table, "one");
  assert(value != NULL && *value == 100);
  value = ht_get(table, "two");
  assert(value != NULL && *value == 200);
  value = ht_get(table, "three"); // Non-existing key
  assert(value == NULL);

  // Update existing key
  assert(ht_insert(table, "one", 150) == 1);
  assert(table->size == 2); // Size should not increase
  value = ht_get(table, "one");
  assert(value != NULL && *value == 150);

  ht_free(table);
  printf("Test insert and get passed.\n");
}

// Test removal
void test_remove() {
  printf("Testing remove (string keys)...\n");
  str_int_table *table = ht_init(INITIAL_CAPACITY);
  assert(table != NULL);

  // Insert some entries
  assert(ht_insert(table, "one", 100) == 1);
  assert(ht_insert(table, "two", 200) == 1);
  assert(table->size == 2);

  // Remove existing key
  assert(ht_remove(table, "one") == 1);
  assert(table->size == 1);
  assert(ht_get(table, "one") == NULL);

  // Remove non-existing key
  assert(ht_remove(table, "three") == 0);
  assert(table->size == 1);

  // Remove last key
  assert(ht_remove(table, "two") == 1);
  assert(table->size == 0);
  assert(ht_get(table, "two") == NULL);

  ht_free(table);
  printf("Test remove passed.\n");
}

// Test resizing (load factor > 70%)
void test_resize() {
  printf("Testing resize (string keys)...\n");
  str_int_table *table = ht_init(INITIAL_CAPACITY); // Capacity = 8
  assert(table != NULL);

  // Insert 6 entries (8 * 0.7 = 5.6, so 6 entries should trigger resize)
  const char *keys[] = {"one", "two", "three", "four", "five", "six", "seven"};
  for (int i = 0; i < 7; i++) {
    assert(ht_insert(table, keys[i], (i + 1) * 100) == 1);
  }
  assert(table->size == 7 );
  assert(table->capacity == 16); // Should have doubled

  // Verify all entries are still accessible
  for (int i = 0; i < 6; i++) {
    int *value = ht_get(table, keys[i]);
    assert(value != NULL && *value == (i + 1) * 100);
  }

  ht_free(table);
  printf("Test resize passed.\n");
}

// Test edge cases
void test_edge_cases() {
  printf("Testing edge cases (string keys)...\n");

  // Null table
  assert(ht_insert(NULL, "one", 100) == 0);
  assert(ht_get(NULL, "one") == NULL);
  assert(ht_remove(NULL, "one") == 0);
  ht_free(NULL); // Should not crash

  // Empty table
  str_int_table *table = ht_init(INITIAL_CAPACITY);
  assert(table != NULL);
  assert(ht_get(table, "one") == NULL);
  assert(ht_remove(table, "one") == 0);

  // Insert with collisions (keys that may hash to the same index)
  assert(ht_insert(table, "abc", 1000) == 1);
  assert(ht_insert(table, "def", 2000) == 1); // May collide depending on hash
  assert(table->size == 2);
  int *value = ht_get(table, "abc");
  assert(value != NULL && *value == 1000);
  value = ht_get(table, "def");
  assert(value != NULL && *value == 2000);

  ht_free(table);
  printf("Test edge cases passed.\n");
}

int main() {
  printf("Running hash table tests (string keys)...\n");
  test_init_free();
  test_insert_get();
  test_remove();
  test_resize();
  test_edge_cases();
  printf("All string key tests passed!\n");
  return 0;
}
