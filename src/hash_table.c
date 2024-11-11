#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#include "hash_table.h"

static ht_item* new_item(const char* k, const char* v) {
    ht_item* item = malloc(sizeof(ht_item));
    item->key = strdup(k);
    item->value = strdup(v);
    return item;
}

static void free_item(ht_item* item) {
    free(item->key);
    free(item->value);
    free(item);
}

hash_table* new_ht(const int size) {
    hash_table* ht = malloc(sizeof(hash_table));

    ht->table_size = size;
    ht->items = calloc(size, sizeof(ht_item));
    return ht;
}

void free_ht(hash_table* ht) {
    for (int i = 0; i < ht->table_size; ++i) {
        ht_item* item_to_delete = ht->items[i];
        if (item_to_delete != NULL) {
            free_item(item_to_delete);
        }
    }
    free(ht->items);
    free(ht);
}

static size_t hash_function(const char* input) { 
    assert(false && "TODO: function hash_function not implemented");
}

void insert_ht(hash_table* ht, const char* k, const char* v) {
    assert(false && "TODO: function insert_ht not implemented");
}

char* search_ht(hash_table* ht, const char* k) {
    assert(false && "TODO: function search_ht not implemented");
}

void delete_ht(hash_table* ht, const char* k) {
    assert(false && "TODO: function delete_ht not implemented");
}


