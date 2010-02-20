#ifndef DSETS_H
#define DSETS_H

typedef struct {
  int size;
  int *father;
  int *rank;
} DSets;

/* dsets_new -- crée une nouvelle collection d'ensembles disjoints
 * de size entiers (entre 0 et size - 1).
 * Renvoie un pointeur sur cette nouvelle collection, ou NULL
 * si la création n'a pas pu se faire.
 * Complexité: O(size)
 */
extern DSets *dsets_new(int size);

/* dsets_free -- détruit une collection d'ensembles et libère la
 * mémoire qu'elle occupait.
 * Complexité: O(1)
 */
extern void dsets_free(DSets *ds);

/* dsets_max_size -- renvoie la taille maximale d'éléments que peut contenir
 * une collection.
 * Complexité: O(1)
 */
extern int dsets_max_size(DSets *ds);

/* dsets_make_set -- crée un nouvel ensemble ne contenant que l'élément n
 * dans la collection ds.
 * Complexité: O(1)
 */
extern void dsets_make_set(DSets *ds, int n);

/* dsets_find_set -- renvoie le représentant de l'ensemble auquel n appartient
 * dans ds.
 * Renvoie -1 si n n'est dans aucun ensemble.
 */
extern int dsets_find_set(DSets *ds, int n);

/* dsets_union -- réuni les ensembles contenant n1 et n2
 * Complexité: O(ln(dsets_max_size(ds)))
 */
extern void dsets_union(DSets *ds, int n1, int n2);

/* dsets_same_set -- détermine si n1 et n2 font parti du meme ensemble
 */
extern int dsets_same_set(DSets *ds, int n1, int n2);

/* dsets_print -- affiche la liste des éléments de ds et de leurs pères
 */
extern void dsets_print(DSets *ds);

#endif /* DSETS_H */
