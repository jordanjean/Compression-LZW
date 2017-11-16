#ifndef COMPRESSION_H
#define COMPRESSION_H
#include <stdio.h>
#include <stdint.h>

#define NB_SEQ 10000
#define AUTOINDEX 256
#define TAILLE_W 10000

void concat(uint8_t chaine[TAILLE_W], int *l_w, uint8_t c);

void compression(FILE *e, FILE *s);

#endif
