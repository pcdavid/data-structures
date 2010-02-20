#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "edge.h"

/* edge_new -- crée une nouvelle arete ayant les caractéristiques
 * spécifiées.
 * Retourne son adress, ou NULL en cas d'échec.
 * Complexité: O(1)
 */
Edge *edge_new(int v1, int v2, float w, int attr)
{
  Edge *tmp;

  assert((v1 >= 0) && (v2 >= 0));
  if ((tmp = (Edge *) malloc(sizeof(Edge))) == NULL) {
#ifdef DEBUG
    fprintf(stderr, "edge_new: erreur d'allocation mémoire.\n");
#endif
    return NULL;
  }
  tmp->v1 = v1;
  tmp->v2 = v2;
  tmp->weight = w;
  tmp->attr = attr;
  return tmp;
}

/* edge_free -- détruit une arete et libère la mémoire qu'elle
 * occupait.
 * Complexite: O(1)
 */
void edge_free(Edge *e)
{
  assert(e != NULL);
  free(e);
}

/* edge_get_weight -- retourne le poids d'une arete.
 * Complexité: O(1)
 */
float edge_get_weight(Edge *e)
{
  assert(e != NULL);
  return e->weight;
}

/* edge_set_weight -- modifie le poids d'une arete.
 * Complexité: O(1)
 */
void edge_set_weight(Edge *e, float w)
{
  assert(e != NULL);
  e->weight = w;
}

/* edge_get_attr -- retourne l'attribut d'une arete.
 * Complexité: O(1)
 */
int edge_get_attr(Edge *e)
{
  assert(e != NULL);
  return e->attr;
}

/* edge_set_attr -- modifie l'attribut d'une arete.
 * Complexité: (1)
 */
void edge_set_attr(Edge *e, int attr)
{
  assert(e != NULL);
  e->attr = attr;
}
    
/* edge_print -- affiche les informations contenues dans e.
 * Complexité: O(1)
 */
void edge_print(Edge *e)
{
  assert(e != NULL);
  printf("%d --(%02d)--> %d\t", e->v1, (int) e->weight, e->v2);
  switch (e->attr) {
  case WHITE:
    printf("Blanc");
    break;
  case GREY:
    printf("Gris");
    break;
  case BLACK:
    printf("Noir");
    break;
  case BLUE:
    printf("Bleu");
    break;
  case RED:
    printf("Rouge");
    break;
  default:
    printf("%d", e->attr);
  }
}
