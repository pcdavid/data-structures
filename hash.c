#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "hash.h"
#include "slist.h"

HashTable *hash_table_new(int max_size, int (*h)(void *), int (*kequal)(void *, void*))
{
  HashTable *tmp;

  if ((max_size == 0) || (h == NULL) || (kequal == NULL) ||
      ((tmp = (HashTable *) malloc(sizeof(HashTable))) == NULL) ||
      ((tmp->items = (SList **) malloc(max_size*sizeof(SList *))) == NULL))
    return NULL;
  tmp->max_size = max_size;
  tmp->size = 0;
  tmp->h = h;
  tmp->kequal = kequal;
  return tmp;
}

void hash_table_free(HashTable *table)
{
  int i;

  if (table == NULL)
    return;
  if (table->items != NULL) {
    for (i = 0; i < table->max_size; i++)
      if (table->items[i] != NULL)
        slist_free(table->items[i]);
    free(table->items);
  }
  free(table);
}

int hash_table_insert(HashTable *table, void *key, void *item)
{
  int k;
  HashItem *tmp;

  if ((key == NULL) || (item == NULL) || hash_table_full(table) ||
      ((tmp = (HashItem *) malloc(sizeof(HashItem))) == NULL))
    return 0;

  k = table->h(key) % table->max_size;
  tmp->key = key;
  tmp->item = item;
  if (table->items[k] == NULL)
    table->items[k] = slist_new();

  return slist_insert(table->items[k], tmp);
}

int hash_table_remove(HashTable *table, void *key)
{
  int k;

  if ((key == NULL) || hash_table_full(table))
    return 0;
  
  k = table->h(key) % table->max_size;
  
}

void *hash_table_find(HashTable *table, void *key)
{
  return 0;
}

int hash_table_max_size(HashTable *table)
{
  return (table == NULL) ? 0 : table->max_size;
}

int hash_table_size(HashTable *table)
{
  return (table == NULL) ?  0 : table->size;
}

int hash_table_empty(HashTable *table)
{
  return (hash_table_size(table) == 0);
}

int hash_table_full(HashTable *table)
{
  return (hash_table_size(table) == hash_table_max_size(table));
}
