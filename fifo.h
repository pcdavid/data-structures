#ifndef FIFO_H
#define FIFO_H

typedef struct {
  int max_size;                 /* Nombre maximale d'éléments */
  int newest, oldest;           /* Pointeurs sur le plus récent et le plus ancien */
  /* oldest est l'indice de l'élément le plus vieux du tableau
   * newest est l'indice de la première case libre (et donc le plus récent est
   * dans la case précédente).
   */
  void **items;                 /* Tableau contenant les éléments */
} Fifo;

/* fifo_new -- crée une nouvelle file pouvant contenir au plus max_size
 * éléments.
 * Retourne un pointeur sur cette file, ou NULL si l'allocation n'a pas
 * pu se faire.
 * Complexité: O(1)
 */

extern Fifo *fifo_new(int max_size);

/* fifo_free -- détruit une file et libère la mémoire qu'elle occupait
 * Complecité: O(1)
 */
extern void fifo_free(Fifo *fifo);

/* fifo_empty -- détermine si une file est vide.
 * Complexité: O(1)
 */
extern int fifo_empty(Fifo *fifo);

/* fifo_full -- détermine si une liste est pleine (ie ne peut plus
 * contenir d'éléments supplémentaires).
 * Complexité: O(1);
 */
extern int fifo_full(Fifo *fifo);

/* fifo_max_size -- retoune le nombre maximum d'éléments qu'une file
 * peut contenir.
 * Complexité: O(1)
 */
extern int fifo_max_size(Fifo *fifo);

/* fifo_length -- retourne la longueur effective d'une file.
 * Complexité: O(1)
 */
extern int fifo_length(Fifo *fifo);

/* fifo_put -- insère un nouvel élément dans la file.
 * Ne fait rien si la file est pleine.
 * Complexité: O(1)
 */
extern void fifo_put(Fifo *fifo, void *item);

/* fifo_get -- retire un élément de la file et renvoie un pointeur
 * sur son contenu.
 * Retourne NULL si la file est vide.
 * Complexité: O(1)
 */
extern void *fifo_get(Fifo *fifo);

/* fifo_print -- affiche une file à l'écran. Chaque élément est affiché en
 * faisant appel à print_item.
 * Complexité: O(1)
 */
extern void fifo_print(Fifo *fifo, void (*print_item)(void *));

#endif /* FIFO_H */
