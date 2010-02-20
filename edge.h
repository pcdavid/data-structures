#ifndef EDGE_H
#define EDGE_H

/* Les différentes couleurs possibles */
#define WHITE 0
#define GREY  1
#define BLACK 2
#define BLUE  3
#define RED   4

typedef struct {
  int v1, v2;
  float weight;
  int attr;
} Edge;

/* edge_new -- crée une nouvelle arete ayant les caractéristiques
 * spécifiées.
 * Retourne son adress, ou NULL en cas d'échec.
 * Complexité: O(1)
 */
extern Edge *edge_new(int v1,int v2, float w, int attr);

/* edge_free -- détruit une arete et libère la mémoire qu'elle
 * occupait.
 * Complexite: O(1)
 */
extern void edge_free(Edge *e);

/* edge_get_weight -- retourne le poids d'une arete.
 * Complexité: O(1)
 */
extern float edge_get_weight(Edge *e);

/* edge_set_weight -- modifie le poids d'une arete.
 * Complexité: O(1)
 */
extern void edge_set_weigth(Edge *e, float w);

/* edge_get_attr -- retourne l'attribut d'une arete.
 * Complexité: O(1)
 */
extern int edge_get_attr(Edge *e);

/* edge_set_attr -- modifie l'attribut d'une arete.
 * Complexité: (1)
 */
extern void edge_set_attr(Edge *e, int attr);

/* edge_print -- affiche les informations contenues dans e.
 * Complexité: O(1)
 */
extern void edge_print(Edge *e);

#endif /* EDGE_H */
