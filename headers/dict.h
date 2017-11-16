#ifndef DICT_H
#define DICT_H
#include <stdint.h>

#define TAILLE_SEQ 100

typedef struct {
        uint8_t *seq;
        int taille;
}dict_seq_t;

typedef dict_seq_t *dict_t;

// Return codes
typedef enum {
        DICT_INFOOK=0,
        DICT_NODICT = 1, // dictionnaire invalide
        DICT_INVALID = 2, // index ou prefixe invalide
        DICT_NOTFOUND = 3, // sequence cherchee absente
        DICT_FOUND = 4, // sequence presente
        DICT_ADDED = 5, // sequence ajoute
        DICT_FULL = 6, // erreur en insertion : plus de place
} dict_error_t;

// Allouer un dictionnaire pour max_elements séquences d'octests
// Fonction desactivee pour autoindex=0
// Retourne NULL en cas d'erreur

dict_t dict_new(int max_elements, int nb_autoindex);

// Rechercher la séquence seq dans le dictionnaire d
// Resultat index

dict_error_t dict_search(dict_t d, uint8_t *seq, int taille_seq, int *index,
                         int nb_autoindex, int max_elements);

// Inserer la séquence seq dans le dictionnaire d
// Resultat index : index de la séquence deja presente ou nouvellement insere

dict_error_t dict_insert(dict_t d, uint8_t *seq, int taille_seq, int *index, int nb_autoindex,
                         int max_elements);
// Lire les informations associees a une sequence
dict_error_t dict_index_content(dict_t d, int index, uint8_t *seq, int *taille_seq, int max_elements);
// Lire les informations associees au dictionnaire
// Last allocated : index de la dernière séquence valide
// Max index      : dernier numero d'index utilisable
dict_error_t dict_info(dict_t d, int *last_allocated, int *max_index,
                       int nb_autoindex, int max_elements);

// Obtention de caractere initial + longueur de la chaine d'un noeud
dict_error_t dict_firstchar_length(dict_t d, int index, uint8_t *carac,
                                   unsigned int *length);

void dict_print(dict_t d, int max_elements);

#endif // DICT_H
