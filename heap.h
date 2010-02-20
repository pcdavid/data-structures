#ifndef HEAP_H
#define HEAP_H

#define heap_left_son(i) (2*((i)+1)-1)
#define heap_right_son(i) (2*((i)+1)+1-1)
#define heap_father(i) ((int) ((i)+1)/2-1) /* FAUX si i == 0 */

typedef int (*relation)(void *, void *);

typedef struct {
  int size;                     /* Taille effective du tas */
  int max;                      /* Taille maximum du tas */
  relation rel;                 /* Relation d'ordre utilisée pour ordonner le tas */
  void **tab;                   /* Les données contenues dans le tas */
} Heap;


/* heap_create -- crée un tas d'une taille maximale donnée, ordonné par
 * la relation rel.
 * Retourne un pointeur sur ce nouveau tas, ou NULL si la création
 * ne s'est pas bien passée.
 * Complexité: O(1)
 */
extern Heap *heap_create(int max_size, relation rel);

/* heap_free -- détruit un tas et libére la mémoire qu'il occupait.
 * Complexité: O(1);
 */
extern void heap_free(Heap *h);

/* heap_root -- renvoie la racine du tas (ie son plus "petit" élément).
 * Ne modifie PAS le tas.
 * Complexité: O(1)
 */
extern void *heap_root(Heap *h);

/* heap_size -- renvoie la taille effective du tas (ie le nombre d'éléments
 * qu'il contient).
 * Complexité: O(1);
 */
extern int heap_size(Heap *h);

/* heap_max_size -- renvoie le nombre maximum d'éléments qu'un tas peut
 * contenir.
 * Complexité: O(1)
 */
extern int heap_max_size(Heap *h);

/* heap_extract_root -- retire le sommet du tas et renvoie sa valeur
 * (en fait un pointeur sur son contenu).
 * Complexité: O(ln(heap_size(h)))
 */
extern void *heap_extract_root(Heap *h);

/* heap_insert -- insère un nouvel élément dans le tas.
 * La valeur de retour indique si l'insertion s'est bien passée.
 * Complexité: O(ln(heap_size(h)))
 */
extern int heap_insert(Heap *h, void *item);

/* heap_print -- affiche le tas à l'écran. Affiche chacun des
 * éléments avec le function print_data.
 * Complexité: O(heap_size(h))
 */
extern void heap_print(Heap *h, void (*print_data)(void *));

#endif /* HEAP_H */
