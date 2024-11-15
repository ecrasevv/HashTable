#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

#include <stddef.h>

typedef struct {
    char*   value;
    char*   key; 
} ht_item;

typedef struct {
    int         table_size;
    ht_item**   items;
} hash_table;

hash_table* new_ht(const int);
void free_ht(hash_table*);
unsigned long djb2_hash_function(const char*, int);
void print_ht(hash_table*);
void insert_ht(hash_table*, const char*, char*);
char* search_ht(hash_table*, const char*);
void delete_ht(hash_table*, const char*);

#endif //HASH_TABLE_H_

