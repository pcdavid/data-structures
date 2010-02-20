#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "heap.h"

/* heap_create -- crée un tas d'une taille maximale donnée, ordonné par
 * la relation rel.
 * Retourne un pointeur sur ce nouveau tas, ou NULL si la création
 * ne s'est pas bien passée.
 * Complexité: O(1)
 */
Heap *heap_create(int max_size, relation rel)
{
  Heap *tmp;

  assert((max_size > 0) && (rel != NULL));
  if ((tmp = (Heap *) malloc(sizeof(Heap))) == NULL) {
#ifdef DEBUG
    fprintf(stderr, "heap_create: erreur d'allocation mémoire.\n");
#endif
    return NULL;
  }
  if ((tmp->tab = (void **) malloc(max_size*sizeof(void *))) == NULL) {
#ifdef DEBUG
    fprintf(stderr, "heap_create: erreur d'allocation mémoire.\n");
#endif
    free (tmp);
    return NULL;
  }
  tmp->size = 0;
  tmp->max = max_size;
  tmp->rel = rel;
  return tmp;
}

/* heap_free -- détruit un tas et libére la mémoire qu'il occupait.
 * Complexité: O(1);
 */
   
void heap_free(Heap *h)
{
  assert(h != NULL);
  if (h->tab != NULL) free (h->tab);
  free (h);
}

/* heap_size -- renvoie la taille effective du tas (ie le nombre d'éléments
 * qu'il contient).
 * Complexité: O(1);
 */
int heap_size(Heap *h)
{
  assert(h != NULL);
  return h->size;
}

/* heap_max_size -- renvoie le nombre maximum d'éléments qu'un tas peut
 * contenir.
 * Complexité: O(1)
 */
int heap_max_size(Heap *h)
{
  assert(h != NULL);
  return h->max;
}

/* heap_root -- renvoie la racine du tas (ie son plus "petit" élément).
 * Ne modifie PAS le tas.
 * Complexité: O(1)
 */
void *heap_root(Heap *h)
{
  assert((h != NULL) && (h->tab != NULL) && (h->size != 0));
  return h->tab[0];
}

/* heap_shift_down (fonction interne) -- fait descendre à sa place dans
 * le tas un élément qui est trop haut.
 * Complexité: O(ln(heap_size(h)))
 */
static void heap_shift_down(Heap *h, unsigned int i)
{
  int next_i;
  int max_i = heap_size(h) - 1; /* Indice à ne pas dépasser */
  void *tmp;

  assert((h != NULL) && (h->tab != NULL) && (i >= 0));
  if (heap_size(h) == 0)
    return;
  while ((heap_left_son(i) <= max_i) || (heap_right_son(i) <= max_i)) {
    /* S'il y en a, détermine le plus grand des fils */
    next_i = heap_left_son(i);
    if ((heap_right_son(i) <= max_i) &&
        (h->rel(h->tab[next_i], h->tab[heap_right_son(i)])))
      next_i = heap_right_son(i);
    /* next_i pointe ici sur le plus grand du ou des fils
     * S'il y a lieu, fait l'échange et continue, sinon sort tout de
     * suite.
     */
    if (h->rel(h->tab[i], h->tab[next_i])) {
      tmp = h->tab[i];
      h->tab[i] = h->tab[next_i];
      h->tab[next_i] = tmp;
      i = next_i;
    } else
      return;
  }
}

/* heap_shift_up (fonction interne) -- fait remonter à sa place dans
 * le tas un élément qui est placé trop bas.
 * Complexite: O(ln(heap_size(h)))
 */
static void heap_shift_up(Heap *h, int i)
{
  int f = heap_father(i);
  void *tmp;

  assert((h != NULL) && (h->tab != NULL) && (i >= 0));
  if (i == 0) return;
  while (h->rel(h->tab[f], h->tab[i])) { /* father < item */
    /* Echange */
    tmp = h->tab[f];
    h->tab[f] = h->tab[i];
    h->tab[i] = tmp;
    /* Remonte les indices */
    i = f;
    if (f == 0)
      return;
    else
      f = heap_father(f);
  }
}

/* heap_extract_root -- retire le sommet du tas et renvoie sa valeur
 * (en fait un pointeur sur son contenu).
 * Complexité: O(ln(heap_size(h)))
 */
void *heap_extract_root(Heap *h)
{
  void *tmp;

  assert((h != NULL) && (h->tab != NULL));
  if (heap_size(h) == 0) return NULL;
  tmp = h->tab[0];
  h->tab[0] = h->tab[h->size-1];
  h->size -= 1;
  heap_shift_down(h, 0);
  return tmp;
}

/* heap_insert -- insère un nouvel élément dans le tas.
 * La valeur de retour indique si l'insertion s'est bien passée.
 * Complexité: O(ln(heap_size(h)))
 */
int heap_insert(Heap *h, void *item)
{
  assert((h != NULL) && (h->tab != NULL) && (item != NULL));
  if (heap_size(h) == heap_max_size(h))
    return 0;
  h->tab[h->size++] = item;
  heap_shift_up(h, h->size-1);
  return 1;
}

/* heap_print -- affiche le tas à l'écran. Affiche chacun des
 * éléments avec le function print_data.
 * Complexité: O(heap_size(h))
 */
void heap_print(Heap *h, void (*print_data)(void *))
{
  int i;

  assert((h != NULL) && (h->tab != NULL) && (print_data != NULL));
  printf("(");
  if ((h != NULL) && (heap_size(h) > 0)) {
    print_data(h->tab[0]);
    for (i = 1; i < heap_size(h); i++) {
      printf(", ");
      print_data(h->tab[i]);
    }
  }
  printf(")");
}
