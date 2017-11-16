#include "binio.h"
#include "compression.h"
#include "dict.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int nombre_de_bit_des_codes = 9;

void concat(uint8_t chaine[TAILLE_W], int *l_w, uint8_t c) {
  int i = 0;
  while (i < TAILLE_W && i < *l_w) {
    i++;
  }
  if (i >= TAILLE_W) {
    fprintf(stderr, "Dépassement de la capacité de W.\n");
    exit(EXIT_FAILURE);
  }
  chaine[i] = c;
  *l_w = *l_w + 1;
  return;
}

void compression(FILE *e, FILE *s) {
  uint8_t w[TAILLE_W];
  uint8_t a;
  int l_w = 0;
  unsigned int init_dict = 0;
  unsigned int nb_max_bits_code = 0;
  int *index = malloc(sizeof(int));
  dict_t d = dict_new(NB_SEQ, AUTOINDEX);
  w[0] = fgetc(e);
  l_w = 1;
  while (!feof(e)) {
    /* Recherche de la plus longue sous-chaine dans d */
    dict_search(d, w, l_w, index, AUTOINDEX, NB_SEQ);
    do {
      a = fgetc(e);
      concat(w, &l_w, a);
    } while (dict_search(d, w, l_w, index, AUTOINDEX, NB_SEQ) != DICT_NOTFOUND);
    /* w est la plus longue sous-chaine presente dans d */
    binio_write(s, *index, nombre_de_bit_des_codes);
    if (dict_insert(d, w, l_w, index, AUTOINDEX, NB_SEQ) == DICT_FULL) {
      fprintf(stderr, "Erreur dictionnaire plein.\n");
      exit(EXIT_FAILURE);
    }
    /* Mise à jour du nombre de bits des codes si besoin est*/
    if (*index >= (1 << nombre_de_bit_des_codes) - 1) {
      nombre_de_bit_des_codes++;
      if (nombre_de_bit_des_codes > nb_max_bits_code) {
        nb_max_bits_code = nombre_de_bit_des_codes;
      }
      binio_write(s, AUTOINDEX + 1, nombre_de_bit_des_codes);
    }
    /* Réinitialisation du dictionnaire si besoin est */
    if (*index >= NB_SEQ - 1) {
      binio_write(s, AUTOINDEX + 2, nombre_de_bit_des_codes);
      d = dict_new(NB_SEQ, AUTOINDEX);
      nombre_de_bit_des_codes = 9;
      init_dict++;
    }
    w[0] = a;
    l_w = 1;
  }
  /* Ecriture du code de fin de fichier */
  binio_write(s, AUTOINDEX, nombre_de_bit_des_codes);
  printf("Dictionnaire réinitialisé %u fois.\n", init_dict);
  printf("Nombre de bits des codes incrémenté jusqu'à %u.\n", nb_max_bits_code);
  return;
}
