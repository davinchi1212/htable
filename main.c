#include "htable.h"
#include <stdio.h> 
#include <string.h>
#include <stdint.h>
#include <stdlib.h> 

DEFINE_HTABLE(const char* , int , Table) ;
int main() {

  Table* table = ht_init(8) ;
  ht_insert(table, "one", 111);
  ht_insert(table, "two", 222);
  ht_insert(table, "three", 333) ;
  for (int i = 0; i < table->capacity; i++) {
    printf(" [%d]  %10s   %d \n", i , table->entries[i].key, table->entries[i].value) ; 
  }
  printf("---------------------\n") ; 
  ht_remove(table, "two") ; 
  int* res = ht_get(table, "two") ; 
  if (res != NULL  ) 
    printf("got res : %d \n", *res) ;
  else
    printf("got null value \n") ; 
  ht_free(table) ; 
  return 0; 
}
