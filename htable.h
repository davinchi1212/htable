#ifndef HTABLE_H_
#define HTABLE_H_

#define INITIAL_CAPACITY 5 
#define DEFINE_HTABLE(KT , KV , NAME)  \
  typedef struct NAME##_entry{         \
    KT key ;                           \
    KV vaulue ;                        \
    int in_use                         \
  }NAME##_entry;                       \

  typedef struct NAME{                 \
    int size  ;                        \
    int capcity ;                      \
    NAME##_entry* entries ;            \
  }NMAE;                               \

  void NAME##_init(NAME* table){       \
    table->capacity = INITIAL_CAPACITY;\
    table->size = 0 ; \
    table->NAME##_entry = calloc(table->capcity , sizeof(NAME##_entry)) ; \
   }\

   void NAME##_free(NAME* table) {\
     free(table->entries) ; \
   }\

  

#endif
