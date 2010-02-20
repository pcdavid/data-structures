#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "fifo.h"

/* fifo_new -- crée une nouvelle file pouvant contenir au plus max_size
 * éléments.
 * Retourne un pointeur sur cette file, ou NULL si l'allocation n'a pas
 * pu se faire.
 * Complexité: O(1)
 */
Fifo *fifo_new(int max_size)
{
    Fifo *tmp;
  
    assert(max_size > 0);
    if ((tmp = (Fifo *) malloc(sizeof(Fifo))) == NULL) {
#ifdef DEBUG
        fprintf(stderr, "fifo_new: erreur d'allocation mémoire.\n");
#endif
        return NULL;
    }
    if ((tmp->items = (void **) malloc((max_size+1)*sizeof(void *))) == NULL) {
#ifdef DEBUG
        fprintf(stderr, "fifo_new: erreur d'allocation mémoire.\n");
#endif
        free(tmp);
        return NULL;
    }
    tmp->max_size = max_size+1;
    tmp->newest = tmp->oldest = 0;

    return tmp;
}

/* fifo_free -- détruit une file et libère la mémoire qu'elle occupait
 * Complexité: O(1)
 */
void fifo_free(Fifo *fifo)
{
    assert((fifo != NULL) && (fifo->items != NULL));
    free(fifo->items);
    free(fifo);
}

/* fifo_max_size -- retoune le nombre maximum d'éléments qu'une file
 * peut contenir.
 * Complexité: O(1)
 */
int fifo_max_size(Fifo *fifo)
{
    assert(fifo != NULL);
    return fifo->max_size-1;
}

/* fifo_length -- retourne la longueur effective d'une file.
 * Complexité: O(1)
 */
int fifo_length(Fifo *fifo)
{
    assert((fifo != NULL) && (fifo->items != NULL));
    if (fifo->newest >= fifo->oldest)
        return fifo->newest - fifo->oldest;
    else
        return fifo->max_size - (fifo->oldest - fifo->newest);
}

/* fifo_empty -- détermine si une file est vide.
 * Complexité: O(1)
 */
int fifo_empty(Fifo *fifo)
{
    assert((fifo != NULL) && (fifo->items != NULL));
    return fifo_length(fifo) == 0;
}

/* fifo_full -- détermine si une liste est pleine (ie ne peut plus
 * contenir d'éléments supplémentaires).
 * Complexité: O(1);
 */
int fifo_full(Fifo *fifo)
{
    assert(fifo != NULL);
    return (fifo_length(fifo) == fifo_max_size(fifo));
}

/* fifo_put -- insère un nouvel élément dans la file.
 * Ne fait rien si la file est pleine.
 * Complexité: O(1)
 */
void fifo_put(Fifo *fifo, void *item)
{
    assert((fifo != NULL) && (fifo->items != NULL) && (item != NULL));
    if (fifo_length(fifo) == fifo_max_size(fifo))
        return;

    fifo->items[fifo->newest++] = item;
    if (fifo->newest == fifo->max_size) fifo->newest = 0;
}

/* fifo_get -- retire un élément de la file et renvoie un pointeur
 * sur son contenu.
 * Retourne NULL si la file est vide.
 * Complexité: O(1)
 */
void *fifo_get(Fifo *fifo)
{
    void *ret;
  
    assert((fifo != NULL) && (fifo->items != NULL));
    if (fifo_empty(fifo))
        return NULL;
    else
        ret = fifo->items[fifo->oldest++];

    if (fifo->oldest == fifo->max_size) fifo->oldest = 0;
    return ret;
}

/* fifo_print -- affiche une file à l'écran. Chaque élément est affiché en
 * faisant appel à print_item.
 * Complexité: O(1)
 */
void fifo_print(Fifo *fifo, void (*print_item)(void *))
{
    int i;

    assert((fifo != NULL) && (print_item != NULL));
    printf("haut<-");
    i = fifo->oldest;
    while (i != fifo->newest) {
        print_item(fifo->items[i]);
        printf("<-");
        i += 1;
        if (i == fifo->max_size) i = 0;
    }
    printf("bas");
}
