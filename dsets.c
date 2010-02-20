#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "dsets.h"

/* dsets_new -- crée une nouvelle collection d'ensembles disjoints
 * de size entiers (entre 0 et size - 1).
 * Renvoie un pointeur sur cette nouvelle collection, ou NULL
 * si la création n'a pas pu se faire.
 * Complexité: O(size)
 */
DSets *dsets_new(int size)
{
  DSets *tmp;
  int i;

  assert(size > 0);
  if ((tmp = (DSets *) malloc(sizeof(DSets))) == NULL) {
#ifdef DEBUG
    fprintf(stderr, "dsets_new: erreur d'allocation mémoire.\n");
#endif
    return 0;
  }

  if ((tmp->father = (int *) malloc(size*sizeof(int))) == NULL) {
#ifdef DEBUG
    fprintf(stderr, "dsets_new: erreur d'allocation mémoire.\n");
#endif
    free(tmp);
    return NULL;
  }

  if ((tmp->rank = (int *) malloc(size*sizeof(int))) == NULL) {
#ifdef DEBUG
    fprintf(stderr, "dsets_new: erreur d'allocation mémoire.\n");
#endif
    return NULL;
  }

  tmp->size = size;
  for (i = 0; i < size; i++) {
    tmp->father[i] = -1;
    tmp->rank[i] = 0;
  }
  return tmp;

}

/* dsets_free -- détruit une collection d'ensembles et libère la
 * mémoire qu'elle occupait.
 * Complexité: O(1)
 */
void dsets_free(DSets *ds)
{
  assert(ds != NULL);
  if (ds->father != NULL) free(ds->father);
  if (ds->rank != NULL) free(ds->rank);
  free(ds);
}

/* dsets_max_size -- renvoie la taille maximale d'éléments que peut contenir
 * une collection.
 * Complexité: O(1)
 */
int dsets_max_size(DSets *ds)
{
  assert(ds != NULL);
  return ds->size;
}

/* dsets_make_set -- crée un nouvel ensemble ne contenant que l'élément n
 * dans la collection ds.
 * Complexité: O(1)
 */
void dsets_make_set(DSets *ds, int n)
{
  assert((ds != NULL) && (ds->father != NULL) && (ds->rank != NULL) && (n >= 0));
  if (n > ds->size) return;
  ds->father[n] = n;
  ds->rank[n] = 0;
}

/* dsets_find_set -- renvoie le représentant de l'ensemble auquel n appartient
 * dans ds.
 * Renvoie -1 si n n'est dans aucun ensemble.
 */
int dsets_find_set(DSets *ds, int n)
{
  assert((ds != NULL) && (ds->father != NULL) && (n >= 0));
  if (n > ds->size) return -1;

  while ((ds->father[ds->father[n]] != ds->father[n]) && (ds->father[n] != -1)) {
    ds->father[n] = ds->father[ds->father[n]],
    n = ds->father[ds->father[n]];
  }
  return ds->father[n];
}

/* dsets_union -- réuni les ensembles contenant n1 et n2
 * Complexité: O(ln(dsets_max_size(ds)))
 */
void dsets_union(DSets *ds, int n1, int n2)
{
  int father, son;

  assert((ds != NULL) && (ds->father != NULL) && (n1 >= 0) && (n2 >= 0));
  if ((n1 > ds->size) || (n2 > ds->size)) return;
  n1 = dsets_find_set(ds, n1);
  n2 = dsets_find_set(ds, n2);
  if ((n1 == -1) || (n2 == -1)) return;

  if (ds->rank[n1] > ds->rank[n2]) {
    father = n1;
    son = n2;
  } else {
    father = n2;
    son = n1;
  }
  ds->father[son] = father;
  if (ds->rank[n1] == ds->rank[n2])
    ds->rank[father] += 1;
}

/* dsets_same_set -- détermine si n1 et n2 font parti du meme ensemble
 */
int dsets_same_set(DSets *ds, int n1, int n2)
{
  int i;
  int j;
  assert((ds != NULL) && (ds->father != NULL) &&
         (ds->rank != NULL) && (n1 >= 0) && (n2 >= 0));
  i = dsets_find_set(ds, n1);
  j = dsets_find_set(ds, n2);
  if ((i == -1) || (j == -1))
    return 0;
  else
    return (i == j);
}

/* dsets_print -- affiche la liste des éléments de ds et de leurs pères
 */
void dsets_print(DSets *ds)
{
  int i;
  int size;
  
  assert(ds != NULL);
  size = dsets_max_size(ds);
  if (size == 0) return;
  for (i = 0; i < size; i++)
    printf("(%d, %d) ", i, dsets_find_set(ds, i));
  putchar('\n');
}
