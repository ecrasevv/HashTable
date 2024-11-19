#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include "hash_table.h"

#define DJB2_INITIAL_VALUE 5381
#define SDBM_INITIAL_VALUE 0
#define MAX_KEY_SIZE       100
#define MAX_VALUE_SIZE     500

#define new_index(index, k, table_size)                               \
    ((index) + sdbm_hash_function((k), (table_size))) % (table_size); \

static ht_item HT_DELETED_ELEMENT = {NULL,NULL};

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

uint32_t djb2_hash_function(const char* input, size_t table_size) 
{ 
    uint32_t hash = DJB2_INITIAL_VALUE;
    int c;
    
    while ((c = *input++)) {
        /* hash = hash * 33 + c ; 2^5 (left shift) */
        hash = ((hash << 5) + hash) + c; 
    }
    return hash % table_size;
}

uint32_t sdbm_hash_function(const char* input, size_t table_size) 
{ 
    uint32_t hash = SDBM_INITIAL_VALUE;
    int c;

    while((c = *input++)) {
        hash = c + (hash << 6) + (hash << 16) - hash;
    }
    return hash % table_size;
}

void insert_ht(hash_table* ht, const char* k, char* v) 
{
    ht_item* item = new_item(k,v);
    uint32_t index = djb2_hash_function(item->key, ht->table_size);
    int i = 0;

    while (ht->items[index] != NULL && ht->items[index] != &HT_DELETED_ELEMENT && i < ht->table_size) {
        index = new_index(index, k, ht->table_size);
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
    uint32_t index = djb2_hash_function(k, ht->table_size);
    int i = 0;

    while (ht->items[index] != NULL && i < ht->table_size) {
            if (ht->items[index] != &HT_DELETED_ELEMENT && strcmp(ht->items[index]->key, k) == 0) {
                return ht->items[index]->value;
            } else {
                index = new_index(index, k, ht->table_size);
                i++;
            }
        }
    return NULL;
}

int delete_ht(hash_table* ht, const char* k)
{
    uint32_t index = djb2_hash_function(k, ht->table_size);
    int i = 0;

    while (ht->items[index] != NULL && i < ht->table_size) {
        if (strcmp(ht->items[index]->key, k) == 0) {
            ht->items[index] = &HT_DELETED_ELEMENT;
            return 0;
        }
        index = new_index(index, k, ht->table_size);
        i++;
    }
    return -1;
}

void insert_from_file(hash_table* ht, ht_item* item)
{
    uint32_t index = djb2_hash_function(item->key, ht->table_size);
    int i = 0;

    while (ht->items[index] != NULL && ht->items[index] != &HT_DELETED_ELEMENT && i < ht->table_size) {
        index = new_index(index, item->key, ht->table_size);
        i++;
    }

    if (i == ht->table_size) {
        printf("table overflow\n");
        free_item(item);
        return;
    }
    ht->items[index] = item;
}

void file_to_hash(hash_table* ht, const char* path_name) 
{
    FILE* file_ptr;
    char* key = malloc(MAX_KEY_SIZE);
    char* value = malloc(MAX_VALUE_SIZE);

    if ((file_ptr = fopen(path_name, "r")) == NULL) {
        perror("fopen fail");
        exit(1);
    }

    ht_item* item = NULL;
    while ((fscanf(file_ptr, "%s %s", key, value)) == 2) {
        item = new_item(key, value);
        insert_from_file(ht, item);
    }

    free(key);
    free(value);
    fclose(file_ptr);
}
