#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "slist.h"

/* slist_new  -- renvoie un pointeur sur une nouvelle list (vide)
 * ou bien NULL en cas d'erreur d'allocation mémoire.
 * Complexité: O(1)
 */
SList *slist_new()
{
  SList *tmp;

  tmp = (SList *) malloc(sizeof(SList));
  if (tmp == NULL) {
#ifdef DEBUG
    fprintf(stderr, "slist_new: erreur d'allocation mémoire.\n");
#endif
    return NULL;
  }
  tmp->current = tmp->first = NULL;
  tmp->index = 0;
  tmp->length = 0;
  return tmp;
}

/* slist_free -- détruit la liste passée en paramètre et libère la
 * mémoire qu'elle occupait. Ne libère pas la mémoire occupée par les
 * données, mais celle occupée par la structure de liste elle-meme.
 * Complexité: O(longueur(list))
 */
void slist_free(SList *list)
{
  Node *n;
  Node *next;

  assert(list != NULL);
  for (n = list->first; n != NULL; n = next) {
    next = n->next;
    free(n);
  }
  free(list);
}

/* slist_empty -- teste si une liste est vide (ie allouée mais ne
 * contenant aucun élément).
 * Complexité: O(1)
 */
int slist_empty(SList *list)
{
  assert(list != NULL);
  return (list->length == 0);
}

/* slist_length -- renvoie la longueur d'une liste
 * Complexité: O(1)
 */
int slist_length(SList *list)
{
  assert(list != NULL);
  return list->length;
}

/* slist_index -- renvoie l'indice du noeud courant dans la
 * liste. Les noeuds sont numérotés de 1 à longueur(liste).
 * Renvoie 0 si la liste est vide.
 * Complexité: O(1)
 */
int slist_index(SList *list)
{
  return slist_empty(list) ? 0 : list->index;
}

/* slist_current -- renvoie un pointeur sur le contenu du noeud courant
 * ou bien NULL si la liste est vide.
 * Si on a dépassé la fin de la liste (avec slist_next), renvoie
 * aussi NULL.
 * Complexité: O(1)
 */
void *slist_current(SList *list)
{
  assert(list != NULL);
  if (slist_empty(list) || (list->current == NULL))
    return NULL;
  else
    return list->current->item;
}

/* slist_reset -- retourne au début de la liste: positionne
 * le noeud courant sur le premier de la liste.
 * Si la liste est vide, ne fait rien.
 * Complexité: O(1)
 */
void slist_reset(SList *list)
{
  assert(list != NULL);
  if (!slist_empty(list)) {
    list->current = list->first;
    list->index = 1;
  }
}

/* slist_next -- avance d'un élément dans la liste et renvoie
 * un pointeur sur le contenu de l'élément d'arrivé.
 * Ne fait rien (et renvoie NULL) si la liste est vide où qu'on est
 * déjà en fin de liste.
 * Complextié: O(1)
 */
void *slist_next(SList *list)
{
  assert(list != NULL);
  if (slist_empty(list) || (list->current == NULL))
    return NULL;
  list->current = list->current->next;
  if (list->current == NULL)
    return NULL;
  else {
    list->index += 1;
    return list->current->item;
  }
}

/* slist_find -- Trouve le premier élément de la liste égal à item
 * (d'après le prédicat pred) et positionne le pointeur de liste dessus.
 * Renvoie un pointeur sur son son contenu.
 * Si l'élément n'a pas pu ^etre trouvé, renvoie NULL.
 * Complexité: O(longueur(list)*C(pred))
 */
void *slist_find(SList *list, void *item, int (*pred)(void *, void *))
{
  assert((list != NULL) && (pred != NULL));
  if (slist_empty(list)) return NULL;

  for (slist_reset(list); list->current != NULL; slist_next(list))
    if (pred(slist_current(list), item)) break;
  if (list->current != NULL)
    return list->current->item; /* Trouvé */
  else
    return NULL;                /* Pas trouvé */
}

/* slist_insert -- insère un élément spécifié à la position courante
 * dans la liste. Les éléments qui suivent se retrouvent décalés d'un cran.
 * Le pointeur de liste est positionnée sur le nouveau noeud.
 * La valeur de retour indique si l'insertion s'est bien passée.
 * pu se faire.
 * Complexité: O(1)
 */
int slist_insert(SList *list, void *item)
{
  Node *new;

  assert((list != NULL) && (item != NULL)); /* Est-ce raisonnable ? */
  if ((new = (Node *) malloc(sizeof(Node))) == NULL) {
#ifdef DEBUG
    fprintf(stderr, "slist_insert: erreur d'allocation mémoire\n");
#endif
    return 0;
  }
  if (slist_empty(list)) {
    list->current = list->first = new;
    list->current->item = item;
    list->current->next = NULL;
    list->index = 1;
    list->length = 1;
  } else {
    if (list->current == NULL)
      slist_reset(list);
    new->next = list->current->next;
    list->current->next = new;
    new->item = list->current->item;
    list->current->item = item;
    list->length += 1;
  }
  return 1;
}

/* slist_insert_sorted -- insère un élément dans la liste.
 * Cette fonction suppose que la liste est trièe selon cmp_func et
 * fait l'insertion du nouvel élément de manière à conserver cette
 * propriété.
 * cmp_func doit ^etre une relation d'ordre total (<= par ex.)
 * Positionne le pointeur de liste sur le nouvel élément inséré.
 * La valeur de retour indique si l'insertion s'est bien passée.
 * Complexité: O(longueur(list)*C(cmp_func))
 */
int slist_insert_sorted(SList *list, void *item, int (*cmp_func)(void *, void *))
{
  Node *new;
  Node *last = NULL;

  assert((list != NULL) && (item != NULL) && (cmp_func != NULL));
  if (slist_empty(list))
    return slist_insert(list, item);

  for (slist_reset(list); list->current != NULL; slist_next(list)) {
    last = list->current;
    if (!cmp_func(slist_current(list), item)) break;
  }
  if (slist_current(list) == NULL) {
    /* On a dépassé la fin de la liste */
    if ((new = (Node *) malloc(sizeof(Node))) == NULL) {
#ifdef DEBUG
      fprintf(stderr, "slist_insert_sorted: erreur d'allocation mémoire\n");
#endif
      return 0;
    }
    new->next = NULL;
    new->item = item;
    last->next = new;
    list->length += 1;
    list->index += 1;
    list->current = new;
    return 1;
  } else
    return slist_insert(list, item);
}

/* slist_remove -- supprime le noeud de la liste sur lequel on se trouve
 * Renvoie un pointeur sur le noeud que l'on vient de supprimer.
 * La valeur de retour indique si la suppression s'est faite.
 * Complexité: O(1) pour tous les noeuds sauf le dernier en O(longueur(list))
 */
int slist_remove(SList *list)
{
  Node *node;

  assert(list != NULL);

  if (slist_empty(list) || (list->current == NULL))
    return 0;

  if (list->current->next != NULL) {
    node = list->current->next->next;
    list->current->item = list->current->next->item;
    free(list->current->next);
    list->current->next = node;
    list->length -= 1;
  } else if (slist_length(list) == 1) {
    /* Liste avec un seul élément */
    free(list->current);
    list->current = list->first = NULL;
    list->length = 0;
    list->index = 0;
  } else {
    /* Suppression du dernier élément d'une liste qui en compte
     * au moins deux.
     */
    node = NULL;
    slist_reset(list);
    while (list->current->next != NULL) {
      node = list->current;
      list->current = list->current->next;
    }
    /* node pointe sur l'avant-dernier noeud */
    free(node->next);
    node->next = NULL;
    list->current = list->first;
    list->index = 1;
    list->length -= 1;
  }
  return 1;
}

/* slist_foreach -- appelle une fonction donnée successivement sur
 * chaque élément de la liste. La fonction à appeler prend 2 paramètres:
 * un pointeur sur l'élément, et un pointeur générique (pour des données
 * utilisateur).
 * Complexité: O(longueur(list)*complexité(func))
 */
void slist_foreach(SList *list, void (*func)(void *, void *), void *data)
{
  assert((list != NULL) && (func != NULL));
  for (slist_reset(list); slist_current(list) != NULL; slist_next(list))
    func(slist_current(list), data);
}

/* slist_print -- affiche une liste sur la sortie standard sous la
 * forme: "(item1, item2, ...)". Fait appel à la fonction print_item pour
 * afficher chacun des éléments.
 * Complexité: O(longueur(list)*C(print_item))
 */
void slist_print(SList *list, void (*print_item)(void *))
{
  assert((list != NULL) && (print_item != NULL));
  putchar('(');
  if (!slist_empty(list)) {
    slist_reset(list);
    print_item(slist_current(list));
    while (slist_next(list) != NULL) {
      printf(", ");
      print_item(slist_current(list));
    }
  }
  putchar(')');
}
