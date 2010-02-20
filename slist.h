#ifndef SLIST_H
#define SLIST_H

typedef struct _node {
  void *item;
  struct _node *next;
} Node;

typedef struct {
  Node *first;
  Node *current;
  int index;
  int length;
} SList;

extern SList *slist_new();
extern void   slist_free(SList *list);

extern int  slist_empty(SList *list);
extern int  slist_length(SList *list);
extern int  slist_index(SList *list);
extern void *slist_current(SList *list);

extern void slist_reset(SList *list);
extern void *slist_next(SList *list);
extern void *slist_find(SList *list, void *item, int (*equal_func)(void *, void *));

extern int slist_insert(SList *list, void *item);
extern int slist_insert_sorted(SList *list, void *item, int (*cmp_func)(void*, void*));
extern int slist_remove(SList *list);

extern void slist_foreach(SList *list, void (*func)(void *, void *), void *data);
extern void slist_print(SList *list, void (*print_item)(void *));

#endif /* SLIST_H */
