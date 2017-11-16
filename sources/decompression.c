#include "binio.h"
#include "compression.h"
#include "decompression.h"
#include "dict.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int nombre_de_bit_des_codes_d = 9;

void decompression(FILE *s, FILE *e) {
  uint8_t a;
  uint8_t w[TAILLE_SEQ];
  int l_w = 0;
  uint8_t w2[TAILLE_SEQ];
  int l_w2 = 0;
  unsigned int init_dict = 0;
  unsigned int nb_max_bits_code = 0;
  int *i = malloc(sizeof(int));
  int *i2 = malloc(sizeof(int));
  int *control = malloc(sizeof(int));
  int *index = malloc(sizeof(int));
  dict_t d = dict_new(NB_SEQ, AUTOINDEX);
  if (binio_read(s, i, nombre_de_bit_des_codes_d) != BINIO_OK) {
    fprintf(stderr, "Erreur binio_read.\n");
    exit(EXIT_FAILURE);
  }
  if (dict_index_content(d, *i, w, &l_w, NB_SEQ) != DICT_INFOOK) {
    fprintf(stderr, "Erreur sequence introuvable.\n");
    exit(EXIT_FAILURE);
  }
  a = w[0];
  fputc(w[0], e);
  while (!feof(s) && *i != AUTOINDEX) {
    /* Réinitialisation du dictionnaire si besoin est */
    if (*i == AUTOINDEX + 2) {
      d = dict_new(NB_SEQ, AUTOINDEX);
      nombre_de_bit_des_codes_d = 9;
      init_dict++;
    }
    if (binio_read(s, i2, nombre_de_bit_des_codes_d) != BINIO_OK) {
      fprintf(stderr, "Erreur binio_read.\n");
      exit(EXIT_FAILURE);
    }
    if (dict_index_content(d, *i2, w2, &l_w2, NB_SEQ) == DICT_NOTFOUND) {
      dict_index_content(d, *i, w2, &l_w2, NB_SEQ);
      concat(w2, &l_w2, a);
    }
    for (int i = 0; i < l_w2; i++) {
      fputc(w2[i], e);
    }
    a = w2[0];
    concat(w, &l_w, a);
    if (dict_insert(d, w, l_w, index, AUTOINDEX, NB_SEQ) == DICT_FULL) {
      fprintf(stderr, "Erreur dictionnaire plein.\n");
      exit(EXIT_FAILURE);
    }
    /* Mise à jour du nombre de bits des codes si besoin est */
    if (*index >= (1 << nombre_de_bit_des_codes_d) - 2) {
      nombre_de_bit_des_codes_d++;
      if (nombre_de_bit_des_codes_d > nb_max_bits_code) {
        nb_max_bits_code = nombre_de_bit_des_codes_d;
      }
      if (binio_read(s, control, nombre_de_bit_des_codes_d) != BINIO_OK) {
        fprintf(stderr, "Erreur binio_read.\n");
        exit(EXIT_FAILURE);
      }
      if (*control != AUTOINDEX + 1) {
        fprintf(stderr,
                "Erreur de controle d'incrementation du nombre de bits des "
                "codes.\n");
        exit(EXIT_FAILURE);
      }
    }
    *i = *i2;
    if (dict_index_content(d, *i, w, &l_w, NB_SEQ) != DICT_INFOOK) {
      fprintf(stderr, "Erreur sequence introuvable.\n");
      exit(EXIT_FAILURE);
    }
  }
  printf("Dictionnaire réinitialisé %u fois.\n", init_dict);
  printf("Nombre de bits des codes incrémenté jusqu'à %u.\n", nb_max_bits_code);
  return;
}
