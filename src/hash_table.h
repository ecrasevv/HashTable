#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

#include <stdint.h>
#include <stddef.h>

typedef struct {
    char* value;
    char* key; 
} ht_item;

typedef struct {
    size_t    table_size;
    ht_item** items;
} hash_table;

hash_table* new_ht(size_t);
void free_ht(hash_table*);
uint32_t djb2_hash_function(const char*, size_t);
uint32_t sdbm_hash_function(const char*, size_t);
void print_ht(hash_table*);
void insert_ht(hash_table*, const char*, char*);
char* search_ht(hash_table*, const char*);
int delete_ht(hash_table*, const char*);

void file_to_hash(hash_table*, const char*);
void insert_from_file(hash_table*, ht_item*);

#endif //HASH_TABLE_H_

