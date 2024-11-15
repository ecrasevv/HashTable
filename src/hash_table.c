#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include "hash_table.h"

static ht_item HT_DELETED = {NULL,NULL};

static ht_item* new_item(const char* k, const char* v)
{
    ht_item* item = malloc(sizeof(ht_item));
    item->key = strdup(k);
    item->value = strdup(v);
    return item;
}

static void free_item(ht_item* item)
{
    free(item->key);
    free(item->value);
    free(item);
}

hash_table* new_ht(size_t size)
{
    hash_table* ht = malloc(sizeof(hash_table));

    ht->table_size = size;
    ht->items = calloc(size, sizeof(ht_item*));
    return ht;
}

void free_ht(hash_table* ht)
{
    for (int i = 0; i < ht->table_size; ++i) {
        ht_item* item_to_delete = ht->items[i];
        if (item_to_delete != NULL) {
            free_item(item_to_delete);
        }
    }
    free(ht->items);
    free(ht);
}

void print_ht(hash_table* ht)
{
    printf("START\n");
    for (int i = 0; i < ht->table_size; ++i) {
        if (ht->items[i] == NULL) {
            printf("\t %i \t -\n", i);
        } else {
            printf("\t %i \t %s \t \n", i, ht->items[i]->value); 
        }
    }
    printf("END\n");
}

unsigned long djb2_hash_function(const char* input, size_t table_size) 
{ 
    unsigned long hash = 5381;
    int c;
    
    while ((c = *input++)) {
        /* hash = hash * 33 + c ; 2^5 (left shift) */
        hash = ((hash << 5) + hash) + c; 
    }
    return hash % table_size;
}

unsigned long sdbm_hash_function(const char* input, size_t table_size) 
{ 
    unsigned long hash = 0;
    int c;

    while((c = *input++)) {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }
    return hash % table_size;
}

void insert_ht(hash_table* ht, const char* k, char* v) 
{
    ht_item* item = new_item(k,v);
    int index = djb2_hash_function(item->key, ht->table_size);
    int i = 0;

    while (ht->items[index] != NULL && i < ht->table_size) {
        index = (index + sdbm_hash_function(item->key, ht->table_size)) % ht->table_size; /* double hashing */
        i++;
    }

    if (i == ht->table_size) {
        printf("table overflow\n");
        free_item(item);
        return;
    }
    ht->items[index] = item;
}

char* search_ht(hash_table* ht, const char* k)
{
    int index = djb2_hash_function(k, ht->table_size);
    int i = 0;

    while (ht->items[index] != NULL && i < ht->table_size) {
        if (strcmp(ht->items[index]->key, k) == 0) {
            return ht->items[index]->value;
        } else {
            index = (index + sdbm_hash_function(k, ht->table_size)) % ht->table_size;
            i++;
        }
    }
    return NULL;
}

void delete_ht(hash_table* ht, const char* k)
{
    assert(false && "TODO: function delete_ht not implemented");
}
