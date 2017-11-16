#include "binio.h"
#include <compression.h>
#include <stdint.h>
#include <stdio.h>

uint32_t buffer = 0;
unsigned int niveau_buffer = 0;

binio_error_t binio_write(FILE *f, int value, unsigned int bitsize) {
  uint8_t w = 0;
  unsigned int niveau_w = 0;
  unsigned int decalage = 0;
  unsigned int nb_bits_value = 8;
  /* Récupération des bits du buffer et des bits de poids fort de
   * value dans le buffer d'écriture w */
  if (niveau_buffer < 8) {
    /* Récupération des niveau_buffer bits du buffer dans w en poids fort */
    if (niveau_buffer != 0) {
      decalage = (32 - (niveau_buffer + (8 - niveau_buffer)));
      w = (uint8_t)(buffer >> decalage);
      niveau_w = niveau_buffer;
      nb_bits_value = 8 - niveau_w;
      niveau_buffer = 0;
      buffer = 0;
    }
    /* Remplissage de w avec les bits de poids fort de value */
    decalage = ((bitsize - 8) + niveau_w);
    w |= (uint8_t)(value >> decalage);
    niveau_w += nb_bits_value;
    /* Récupération du surplus de bits de value dans buffer */
    decalage = (32 - bitsize) + nb_bits_value;
    buffer = (value << decalage);
    niveau_buffer = (bitsize - nb_bits_value);
    /* Ecriture du buffer w */
    // printf(
    //  "value = %d, w = %x, buffer = %x, niveau_buffer = %d, bitsize = %d\n",
    //  value, w, buffer, niveau_buffer, bitsize);
    fputc(w, f);
  } else {
    return BINIO_BADVALUE;
  }
  /* Vidage du surplus de bits du buffer */
  while (niveau_buffer >= 8) {
    w = 0;
    w = (uint8_t)(buffer >> (32 - 8));
    buffer = (buffer << 8);
    niveau_buffer -= 8;
    /* Ecriture du buffer w */
    //  printf(
    //    "value = %d, w = %x, buffer = %x, niveau_buffer = %d, bitsize = %d\n",
    //  value, w, buffer, niveau_buffer, bitsize);
    fputc(w, f);
  }
  /* Vidage du buffer et éventuelle mise en place de bits de bourrage en fin de
   * fichier */
  if (value == AUTOINDEX && niveau_buffer > 0) {
    w = (uint8_t)(buffer >> (32 - 8));
    //  printf("END -> w = %x, buffer = %x, padding = %d, bitsize = %d\n", w,
    //       buffer, 8 - niveau_buffer, bitsize);
    fputc(w, f);
  }
  return BINIO_OK;
}

binio_error_t binio_read(FILE *f, int *value, unsigned int bitsize) {
  uint8_t r = 0;
  uint32_t masque = 0;
  /* Ecriture du prochain entier du fichier ouvert dans f dans value sur
   * bitsize-bits */
  while (niveau_buffer < bitsize) {
    r = fgetc(f);
    masque = 0;
    masque = (uint32_t)r;
    masque = (masque << (32 - 8 - niveau_buffer));
    buffer = (buffer | masque);
    niveau_buffer += 8;
  }
  if (niveau_buffer > 32) {
    return BINIO_BADVALUE;
  }
  *value = (buffer >> (32 - bitsize));
  // printf("r = %d, buffer = %x, niveau_buffer = %d, bitsize = %d\n", *value,
  //       buffer, niveau_buffer, bitsize);
  buffer = (buffer << bitsize);
  niveau_buffer = niveau_buffer - bitsize;
  return BINIO_OK;
}
