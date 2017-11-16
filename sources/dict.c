#include "dict.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Allouer un dictionnaire pour max_elements séquences d'octests
// Fonction desactivee pour autoindex=0
// Retourne NULL en cas d'erreur

dict_t dict_new(int max_elements, int nb_autoindex) {
  int i, j;
  if (nb_autoindex == 0) {
    return NULL;
  }
  dict_t dictionnaire = (dict_seq_t *)malloc(max_elements * sizeof(dict_seq_t));
  for (i = 0; i < nb_autoindex; i++) {
    dictionnaire[i].seq = malloc(sizeof(uint8_t));
    dictionnaire[i].seq[0] = (uint8_t)i;
    dictionnaire[i].taille = 1;
  }
  /* Code de fin de contenu */
  dictionnaire[i].seq = malloc(sizeof(uint8_t));
  dictionnaire[i].seq[0] = (uint8_t)0;
  dictionnaire[i].taille = 0;
  /* Code d'incrémentation de la fin des codes */
  dictionnaire[i + 1].seq = malloc(sizeof(uint8_t));
  dictionnaire[i + 1].seq[0] = (uint8_t)0;
  dictionnaire[i + 1].taille = 0;
  /* Code de réinitialisation du dictionnaire */
  dictionnaire[i + 2].seq = malloc(sizeof(uint8_t));
  dictionnaire[i + 2].seq[0] = (uint8_t)0;
  dictionnaire[i + 2].taille = 0;
  for (j = nb_autoindex + 3; j < max_elements; j++) {
    dictionnaire[j].seq = NULL;
    dictionnaire[j].taille = 0;
  }
  return dictionnaire;
}

// Rechercher la séquence seq dans le dictionnaire d
// Resultat index : index de la case

dict_error_t dict_search(dict_t d, uint8_t *seq, int taille_seq, int *index,
                         int nb_autoindex, int max_elements) {
  int i = 0;
  int j;
  while (i < max_elements && d[i].seq != NULL) {
    if ((i < nb_autoindex || i > nb_autoindex + 2) &&
        d[i].taille == taille_seq) {
      j = 0;
      while (j < taille_seq && d[i].seq[j] == seq[j]) {
        j++;
      }
      if (j == taille_seq) {
        *index = i;
        return DICT_FOUND;
      }
    }
    i++;
  }
  return DICT_NOTFOUND;
}

// Inserer la séquence seq dans le dictionnaire d
// Resultat index : index de la séquence deja present ou nouvellement insere

dict_error_t dict_insert(dict_t d, uint8_t *seq, int taille_seq, int *index,
                         int nb_autoindex, int max_elements) {
  if (dict_search(d, seq, taille_seq, index, nb_autoindex, max_elements) ==
      DICT_NOTFOUND) {
    int i = nb_autoindex + 3;
    while (i < max_elements && d[i].seq != NULL) {
      i++;
    }
    if (i >= max_elements) {
      return DICT_FULL;
    }
    d[i].seq = malloc(sizeof(uint8_t) * taille_seq);
    *index = i;
    for (int j = 0; j < taille_seq; j++) {
      d[i].seq[j] = seq[j];
    }
    d[i].taille = taille_seq;
    return DICT_ADDED;
  }
  return DICT_FOUND;
}

// Lire les informations associees a une sequence
dict_error_t dict_index_content(dict_t d, int index, uint8_t *seq,
                                int *taille_seq, int max_elements) {
  if (index < max_elements && d[index].seq != NULL) {
    *taille_seq = d[index].taille;
    for (int i = 0; i < d[index].taille; i++) {
      seq[i] = d[index].seq[i];
    }
    return DICT_INFOOK;
  }
  return DICT_NOTFOUND;
}

// Lire les informations associees au dictionnaire
// Last allocated : index de la dernière séquence valide
// Max index      : dernier numero d'index utilisable
dict_error_t dict_info(dict_t d, int *last_allocated, int *max_index,
                       int nb_autoindex, int max_elements) {
  int i = nb_autoindex + 3;
  int pred = i;
  while (i < max_elements && d[i].seq != NULL) {
    pred = i;
    i++;
  }
  if (i >= max_elements) {
    return DICT_FULL;
  }
  *last_allocated = pred;
  *max_index = i;
  return DICT_INFOOK;
}

// Obtention de caractere initial + longueur de la chaine d'une séquence
dict_error_t dict_firstchar_length(dict_t d, int index, uint8_t *carac,
                                   unsigned int *length) {
  if (d[index].seq == NULL) {
    return DICT_NOTFOUND;
  }
  *length = d[index].taille;
  *carac = d[index].seq[0];
  return DICT_INFOOK;
}

void dict_print(dict_t d, int max_elements) {
  for (int i = 0; i < max_elements; i++) {
    printf("%d -> taille = %d seq = ", i, d[i].taille);
    for (int j = 0; j < d[i].taille; j++) {
      printf("%c", d[i].seq[j]);
    }
    printf("\n");
  }
}
