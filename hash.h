#ifndef HASH_H
#define HASH_H
#include "slist.h"

typedef struct {
  void *key;
  void *item;
} HashItem;

typedef struct {
  int max_size;
  int size;
  int (*h)(void *);             /* Convertie une clé en entier */
  int (*kequal)(void *, void*);  /* Prédicat de comparaison de clés */
  SList **items;
} HashTable;

extern HashTable *hash_table_new(int max_size, int (*h)(void *), int (*kequal)(void *, void *));
extern void hash_table_free(HashTable *table);
extern int hash_table_insert(HashTable *table, void *key, void *item);
extern int hash_table_remove(HashTable *table, void *item);
extern void *hash_table_find(HashTable *table, void *key);
extern int hash_table_max_size(HashTable *table);
extern int hash_table_size(HashTable *table);
extern int hash_table_empty(HashTable *table);
extern int hash_table_full(HashTable *table);

#endif /* HASH_H */
