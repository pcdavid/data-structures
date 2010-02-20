#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "graph.h"

/* graph_new -- crée un nouveau graphe non orienté (vide) pouvant
 * contenir au maximum size sommets.
 * Retourne l'adresse de ce nouveau graphe, ou NULL en cas d'échec.
 * Complexité: O(1)
 */
Graph *graph_new(int size)
{
  Graph *tmp;

  assert(size > 0);
  if ((tmp = (Graph *) malloc(sizeof(Graph))) == NULL) {
#ifdef DEBUG
    fprintf(stderr, "graph_new: erreur d'allocation mémoire.\n");
#endif
    return NULL;
  }
  tmp->size = size;
  if ((tmp->vertex = (Vertex *) malloc(size*sizeof(Edge *))) == NULL) {
#ifdef DEBUG
    fprintf(stderr, "graph_new: erreur d'allocation mémoire.\n");
#endif
    return NULL;
  }
  return tmp;
}

/* graph_free -- détruit un graphe et libère la mémoire qu'il occupait.
 * Complexité: O(g->size^2) 
 */
void graph_free(Graph *g)
{
  int i, j;

  assert((g != NULL) && (g->vertex != NULL) && (g->size > 0));

  for (i = 0; i < g->size; i++)
    if (g->vertex[i] != NULL) {
      for (j = 0; j < g->size; j++)
        if (g->vertex[i][j] != NULL) {
          edge_free(g->vertex[i][j]);
          g->vertex[i][j] = g->vertex[j][i] = NULL;
        }
      free(g->vertex[i]);
    }
  free(g->vertex);
  free(g);
}

/* graph_empty -- détermine si un graphe est vide (ie ne contient
 * aucun sommet).
 * Complexité: O(g->size)
 */
int graph_empty(Graph *g)
{
  int i;

  assert((g != NULL) && (g->size > 0) && (g->vertex != NULL));
  for (i = 0; i < g->size; i++)
    if (g->vertex[i] != NULL) break;
  return i == g->size;
}

/* graph_size -- détermine la taille (ie le nombre de sommets) d'un
 * graphe.
 * Complexité: O(g->size)
 */
int graph_size(Graph *g)
{
  int i, n;

  assert((g != NULL) && (g->size > 0) && (g->vertex != NULL));
  for (i = 0, n = 0; i < g->size; i++)
    if (g->vertex[i] != NULL) n += 1;
  return n;
}

/* graph_max_size -- détermine le nombre maximum de sommets qu'un
 * graphe peut contenir.
 * Complexité: O(1)
 */
int graph_max_size(Graph *g)
{
  assert( g != NULL);
  return g->size;
}

/* graph_has_vertex -- détermine si un graphe possède un certain
 * sommet v.
 * Complexité: O(1)
 */
int graph_has_vertex(Graph *g, int v)
{
  assert((g != NULL) && (g->size > 0) && (g->vertex != NULL) && (v >= 0));
  return (v >= g->size) ? 0 : (g->vertex[v] != NULL);
}

/* graph_has_edge -- détermine si un graphe a une certaine arete (v1, v2)
 * Complexité: O(1)
 */
int graph_has_edge(Graph *g, int v1, int v2)
{
  assert((g != NULL) && (g->vertex != NULL) && (v1 >= 0) && (v2 >= 0));
  if (! ((graph_has_vertex(g, v1)) && (graph_has_vertex(g, v2))))
    return 0;
  return (g->vertex[v1][v2] != NULL);
}

/* graph_vertex_degree -- renvoie le degré du sommet v.
 * Complexité: O(g->size)
 */
int graph_vertex_degree(Graph *g, int v)
{
  int i, deg;

  assert((g != NULL) && (v >= 0));
  if (! graph_has_vertex(g, v)) return 0;

  for (deg = 0, i = 0; i < g->size; i++)
    if (graph_has_edge(g, v, i)) deg += 1;
  return deg;
}

/* graph_first_edge -- retourne la première arete de g,
 * ou NULL si g n'a pas d'arete.
 * Complexité: O(g->size)
 */
Edge *graph_first_edge(Graph *g)
{
  Edge *tmp = NULL;
  int i, j;

  assert((g != NULL) && (g->vertex != NULL));
  for (i = 0; i < g->size; i++) {
    if (g->vertex[i] != NULL)
      for (j = 0; j < g->size; j++) {
        if (g->vertex[i][j] != NULL) {
          tmp = g->vertex[i][j];
          break;
        }
      }
    if (tmp != NULL) break;
  }
  return tmp;
}

/* graph_next_edge -- retourne l'arete suivant e dans g, ou NULL
 * si e est la dernière.
 * Les appels sucessifs ne renvoient jamais deux fois la meme arete.
 * Complexité: O(g->size)
 */
Edge *graph_next_edge(Graph *g, Edge *e)
{
  int i, j;

  assert((g != NULL) && (g->vertex != NULL) && (e != NULL));
  if ((e->v1 >= g->size) || (e->v2 >= g->size))
    return NULL;

  if (e->v1 >= e->v2) {
    i = e->v1; j = e->v2;
  } else {
    i = e->v2; j = e->v1;
  }
  if (++j >= i) {               /* Diagonale ? */
    /* Passage à la ligne suivante */
    i += 1;
    j = 0;
  }
  while ((i < g->size) && (g->vertex[i][j] == NULL)) {
    if (++j >= i) {               /* Diagonale ? */
      /* Passage à la ligne suivante */
      i += 1;
      j = 0;
    }
  }
  if (i >= g->size)
    return NULL;
  else
    return graph_get_edge(g, i, j);
}

/* graph_get_edge -- renvoie un pointeur sur l'arete spécifiée
 * ou NULL si elle n'existe pas.
 * Complexité: O(1)
 */
Edge *graph_get_edge(Graph *g, int v1, int v2)
{
  assert((g != NULL) && (v1 >= 0) && (v2 >= 0));
  if (!graph_has_edge(g, v1, v2))
    return NULL;
  else
    return g->vertex[v1][v2];
}

/* graph_add_vertex -- ajoute le sommet v au graphe g.
 * Ne fait rien si le sommet existe déjà.
 * La valeur de retour indique si l'insertion s'est bien
 * passée.
 * Complexité: O(1)
 */
int graph_add_vertex(Graph *g, int v)
{
  assert((g != NULL) && (g->vertex != NULL) && (v >= 0));
  if (v >= graph_max_size(g)) return 0;

  if (g->vertex[v] == NULL) {
    g->vertex[v] = (Edge **) malloc(g->size*sizeof(Edge *));
    if (g->vertex[v] == NULL) {
#ifdef DEBUG
      fprintf(stderr, "graph_add_vertex: erreur d'allocation mémoire.\n");
#endif
      return 0;
    }
  }
  return 1;
}

/* graph_add_edge -- ajoute une arete reliant v1 et v2, avec le
 * poids w et l'attribut attr au graphe g.
 * La valeur de retour indique si l'opération s'est bien déroulée.
 * Complexité: O(1)
 */
int graph_add_edge(Graph *g, int v1, int v2, float w, int attr)
{
  Edge *e;

  assert((g != NULL) && (g->vertex != NULL) && (v1 >= 0) && (v2 >= 0));
  if ((v1 >= graph_max_size(g)) || (v2 >= graph_max_size(g)))
    return 0;
  if (!graph_has_vertex(g, v1)) graph_add_vertex(g, v1);
  if (!graph_has_vertex(g, v2)) graph_add_vertex(g, v2);

  if ((e = edge_new(v1, v2, w, attr)) == NULL) {
#ifdef DEBUG
    fprintf(stderr, "graph_add_edge: erreur d'allocation mémoire\n");
#endif
    return 0;
  }
  g->vertex[v1][v2] = e;
  g->vertex[v2][v1] = e;
  return 1;
}

/* graph_remove_edge -- retire l'arete (v1, v2) du graphe g.
 * La valeur de retour indique si l'opération s'est bien passée.
 * (une valeur 0 indique que l'arete n'existait pas)
 * Complexité: O(1)
 */
int graph_remove_edge(Graph *g, int v1, int v2)
{
  assert((g != NULL) && (g->vertex != NULL));
  if (!graph_has_vertex(g, v1) || !graph_has_vertex(g, v2))
    return 0;
  edge_free(graph_get_edge(g, v1, v2));
  g->vertex[v1][v2] = g->vertex[v2][v1] = NULL;
  return 1;
}

/* graph_remove_vertex -- retire le sommet v du graph g.
 * La valeur de retour indique si l'opération s'est bien passée.
 * Complexité: O(g->size)
 */
int graph_remove_vertex(Graph *g, int v)
{
  int i;

  assert((g != NULL) && (g->vertex != NULL) && (v >= 0));
  if (! graph_has_vertex(g, v)) return 0;

  for (i = 0; i < g->size; i++)
    if (graph_has_edge(g, i, v)) graph_remove_edge(g, i, v);
  free(g->vertex[v]);
  g->vertex[v] = NULL;
  return 1;
}


/* graph_ugly_print -- affiche le graphe à l'écran (sous une forme
 * pas très lisible).
 * Complexité: O((g->size)^2)
 */
void graph_ugly_print(Graph *g)
{
  int i, j;

  assert((g != NULL) && (g->vertex != NULL));
  for (i = 0; i < g->size; i++) {
    if (g->vertex[i] == NULL) continue;
    printf("Sommet %3d: ", i);
    for (j = 0; j < g->size; j++)
      if (g->vertex[i][j] != NULL) {
        printf("(%d ", j);
        edge_print(graph_get_edge(g, i, j));
        printf(") ");
      }
    putchar('\n');
  }
}
  
