#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "htable.h"

// Define a struct for keys
typedef struct {
  int x;
  int y;
} Point;

// Define a hash table with Point keys and int values
DEFINE_HTABLE(Point, int, point_int_table);

// Hash function for Point keys (FNV-1a)
//uint64_t ht_hash(Point key) {
//  uint64_t hash = OFFSET_HASH;
//  // Hash x
//  for (int i = 0; i < sizeof(int); i++) {
//    hash ^= (key.x >> (i * 8)) & 0xFF;
//    hash *= FNV_PRIME;
//  }
//  // Hash y
//  for (int i = 0; i < sizeof(int); i++) {
//    hash ^= (key.y >> (i * 8)) & 0xFF;
//    hash *= FNV_PRIME;
//  }
//  return hash;
//}

// Test initialization and freeing
void test_init_free() {
  printf("Testing init and free (struct keys)...\n");
  point_int_table *table = ht_init(INITIAL_CAPACITY);
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
  printf("Testing insert and get (struct keys)...\n");
  point_int_table *table = ht_init(INITIAL_CAPACITY);
  assert(table != NULL);

  Point p1 = {1, 2};
  Point p2 = {3, 4};

  // Insert key-value pairs
  assert(ht_insert(table, p1, 100) == 1);
  assert(table->size == 1);
  assert(ht_insert(table, p2, 200) == 1);
  assert(table->size == 2);

  // Retrieve values
  int *value = ht_get(table, p1);
  assert(value != NULL && *value == 100);
  value = ht_get(table, p2);
  assert(value != NULL && *value == 200);
  Point p3 = {5, 6}; // Non-existing key
  value = ht_get(table, p3);
  assert(value == NULL);

  // Update existing key
  assert(ht_insert(table, p1, 150) == 1);
  assert(table->size == 2); // Size should not increase
  value = ht_get(table, p1);
  assert(value != NULL && *value == 150);

  ht_free(table);
  printf("Test insert and get passed.\n");
}

// Test removal
void test_remove() {
  printf("Testing remove (struct keys)...\n");
  point_int_table *table = ht_init(INITIAL_CAPACITY);
  assert(table != NULL);

  Point p1 = {1, 2};
  Point p2 = {3, 4};

  // Insert some entries
  assert(ht_insert(table, p1, 100) == 1);
  assert(ht_insert(table, p2, 200) == 1);
  assert(table->size == 2);

  // Remove existing key
  assert(ht_remove(table, p1) == 1);
  assert(table->size == 1);
  assert(ht_get(table, p1) == NULL);

  // Remove non-existing key
  Point p3 = {5, 6};
  assert(ht_remove(table, p3) == 0);
  assert(table->size == 1);

  // Remove last key
  assert(ht_remove(table, p2) == 1);
  assert(table->size == 0);
  assert(ht_get(table, p2) == NULL);

  ht_free(table);
  printf("Test remove passed.\n");
}

// Test resizing (load factor > 70%)
void test_resize() {
  printf("Testing resize (struct keys)...\n");
  point_int_table *table = ht_init(INITIAL_CAPACITY); // Capacity = 8
  assert(table != NULL);

  // Insert 6 entries (8 * 0.7 = 5.6, so 6 entries should trigger resize)
  Point points[] = {{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}, {7, 77}};
  for (int i = 0; i < 7; i++) {
    assert(ht_insert(table, points[i], (i + 1) * 100) == 1);
  }
  assert(table->size == 7);
  assert(table->capacity == 16); // Should have doubled

  // Verify all entries are still accessible
  for (int i = 0; i < 6; i++) {
    int *value = ht_get(table, points[i]);
    assert(value != NULL && *value == (i + 1) * 100);
  }

  ht_free(table);
  printf("Test resize passed.\n");
}

// Test edge cases
void test_edge_cases() {
  printf("Testing edge cases (struct keys)...\n");

  // Null table
  Point p = {1, 2};
  assert(ht_insert(NULL, p, 100) == 0);
  assert(ht_get(NULL, p) == NULL);
  assert(ht_remove(NULL, p) == 0);
  ht_free(NULL); // Should not crash

  // Empty table
  point_int_table *table = ht_init(INITIAL_CAPACITY);
  assert(table != NULL);
  assert(ht_get(table, p) == NULL);
  assert(ht_remove(table, p) == 0);

  // Insert with collisions (points that may hash to the same index)
  Point p1 = {1, 1};
  Point p2 = {9, 9}; // May collide depending on hash
  assert(ht_insert(table, p1, 1000) == 1);
  assert(ht_insert(table, p2, 2000) == 1);
  assert(table->size == 2);
  int *value = ht_get(table, p1);
  assert(value != NULL && *value == 1000);
  value = ht_get(table, p2);
  assert(value != NULL && *value == 2000);

  ht_free(table);
  printf("Test edge cases passed.\n");
}

int main() {
  printf("Running hash table tests (struct keys)...\n");
  test_init_free();
  test_insert_get();
  test_remove();
  test_resize();
  test_edge_cases();
  printf("All struct key tests passed!\n");
  return 0;
}
