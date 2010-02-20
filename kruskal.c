#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* Fonctions de manipulation des graphes */
#include "edge.h"
#include "graph.h"

/* Structures de données nécessaires pour les algorithmes */
#include "dsets.h"
#include "heap.h"
#include "fifo.h"

/* Permet de suivre le déroulement des algorithmes (si verbose != 0) */
static int verbose = 0;

/* La relation utilisée pour ordonner les aretes dans le tas */
int edge_relation(void *e1, void *e2)
{
  assert((e1 != NULL) && (e2 != NULL));
  return (((Edge *) e1)->weight >= ((Edge *) e2)->weight);
}

/* kruskal -- Applique l'algorithme de Kruskal sur le graphe g. En
 * sortie, les aretes de g qui appartiennent à l'Arbre Couvrant
 * Minimum sont coloriées en bleu (attr == BLUE) , et les autres en
 * rouge (attr == RED).
 */
void kruskal(Graph *g)
{
  Heap *h;                      /* Tas utilisé pour trier les aretes */
  DSets *groups;                /* Groupes de sommets reliés */
  Edge *current;                /* Arete en cours de traitement */
  int size = graph_max_size(g); /* Taille de g (nb de sommets) */
  int i;

  assert((g != NULL) && (size > 0));
  if (verbose) printf("Entrée dans kruskal:\n\tInitialisations...\n");

  /* Crée les structures */
  /* Note: la taille de h (size*size) n'est pas optimale, mais
   * le calcul du nombre exact d'aretes prendrait trop de temps.
   */
  if (((h = heap_create(size*size, edge_relation)) == NULL) ||
      ((groups = dsets_new(size)) == NULL)) {
    fprintf(stderr, "kruskal: erreur d'allocation mémoire.\n");
    exit(1);
  }

  /* Initialise les groupes de sommets */
  for (i = 0; i < size; i++)
    if (graph_has_vertex(g, i))
      dsets_make_set(groups, i);

  /* Remplie le tas avec les aretes et les colorie en rouge */
  for (current = graph_first_edge(g);
       current != NULL;
       current = graph_next_edge(g, current))
    {
      heap_insert(h, (void *) current);
      edge_set_attr(current, RED);
    }

  if (verbose) printf("\tDébut du traitement.\n");

  /* Traite les aretes dans l'ordre */
  while (heap_size(h) != 0) {
    current = (Edge *) heap_extract_root(h);
    if (verbose) {
      printf("\tArete faible: ");
      edge_print(current);
      putchar('\n');
    }
    if (!dsets_same_set(groups, current->v1, current->v2)) {
      edge_set_attr(current, BLUE);
      dsets_union(groups, current->v1, current->v2);
      if (verbose) {
        printf("\tArete sélectionnée => ");
        printf("Union: %d U %d\n", current->v1, current->v2);
      }
    }
  }
  if (verbose) printf("Sortie de kruskal.\n");
  heap_free(h);
  dsets_free(groups);
}

/* print_edges -- affiche les aretes de g sous une forme à peu près
 * lisible.
 */
void print_edges(Graph *g)
{
  Edge *current;

  assert(g != NULL);

  for (current = graph_first_edge(g);
       current != NULL;
       current = graph_next_edge(g, current))
    {
      edge_print(current);
      putchar('\n');
    }
}

void bfs_visit(Graph *g, int i, int mark[], int parent[])
{
  int size = graph_max_size(g);
  Vertex u;
  int k, n;
  Fifo *f = fifo_new(size);
  
  if (verbose) 
    printf("\tEntrée dans BFS_visit (sommet %d):\n", i);

  mark[i] = 1;
  fifo_put(f, (void *) g->vertex[i]);
  
  while (!fifo_empty(f)) {
    /* Retire un sommet */
    u = (Vertex) fifo_get(f);
    /* Parcours ses successeurs */
    for (k = 0; k < size; k++)
      if (u[k] != NULL) {
        if ((!mark[k]) && (edge_get_attr(u[k]) == BLUE)) {
          /* Calcule le numéro du sommet en cours */
          n = (u[k]->v1 == k) ? u[k]->v2 : u[k]->v1;
          /* Enfile le successeur */
          fifo_put(f, (void *) g->vertex[k]);
          /* Traitement */
          mark[k] = 1;
          parent[k] = n;
          if (verbose) {
            printf("\t\tInsertion dans la file: %d\n", k);
            printf("\t\tpère[%d] <= %d\n", k, n);
          }
        }
      }
  }
  if (verbose) printf("\tSortie de BFS_visit.\n");
}

/* bfs -- effectue un parcours BFS de g avec mise
 * à jour du tableau des pères.
 */
void bfs(Graph *g, int *parent)
{
  int i, size = graph_max_size(g);
  int mark[size];

  if (verbose) printf("Entrée dans BFS:\n\tInitialisations...\n");

  for (i = 0; i < size; i++) {
    mark[i] = 0;
    parent[i] = -1;
  }
    
  if (verbose) printf("\tDébut du traitement.\n");
  for (i = 0; i < size; i++) {
    if (graph_has_vertex(g, i) && (!mark[i]))
      bfs_visit(g, i, mark, parent);
  }
  if (verbose) printf("Sortie de BFS.\n");
}


int main(int argc, char *argv[])
{
  Graph *g;
  int *parent;
  char line[30];
  int i, size, v1, v2;
  float w;

  /* Lecture des arguments */
  if ((argc == 2) && strcmp(argv[1], "-v") == 0)
    verbose = 1;

  /* Première ligne: nombre de sommets */
  fgets(line, 30, stdin);
  sscanf(line, "%d", &size);
  if ((g = graph_new(size)) == NULL) {
    fprintf(stderr, "kruskal: erreur de la création du graphe (%d sommets).\n", size);
    exit(1);
  }
  parent = (int *) malloc(size*sizeof(int));

  /* Lectures des aretes */
  while (fgets(line, 30, stdin)) {
    if (sscanf(line, "%d %d %f\n", &v1, &v2, &w) == EOF) {
      fprintf(stderr, "kruksal: erreur dans le format du fichier d'éntrée:\n");
      fprintf(stderr, "%s", line);
      exit(1);
    }
    if (!graph_add_edge(g, v1, v2, w, WHITE)) {
      fprintf(stderr, "kruskal: erreur lors de la création de l'arete (%d, %d).\n",
              v1, v2);
      exit(1);
    }
  }

  /* Début du traitement */
  kruskal(g);
  printf("\nRésultat après Kruskal:\n");
  print_edges(g);
  putchar('\n');

  bfs(g, parent);
  printf("\nTableau des pères (après BFS):\n");
  for (i = 0; i < size; i++)
    printf("père[%d] = %d\n", i, parent[i]);

  /* Libération des ressources */
  graph_free(g);
  return 0;
}
