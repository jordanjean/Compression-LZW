#include "compression.h"
#include "decompression.h"
#include "dict.h"
#include <stdio.h>
#include <string.h>

#include "binio.h"
#include <stdlib.h>

int main(int argc, char const *argv[]) {
  if (argc != 4) {
    perror("Usage : -c/-d file_in file_out");
    return 1;
  }
  FILE *e = fopen(argv[2], "r");
  FILE *s = fopen(argv[3], "w");
  if (s == NULL || e == NULL) {
    perror("Erreur a l'ouverture");
    return 1;
  }
  if (strcmp(argv[1], "-c") == 0) {
    compression(e, s);
    fclose(e);
    fclose(s);
  } else if (strcmp(argv[1], "-d") == 0) {
    decompression(e, s);
    fclose(e);
    fclose(s);
  } else {
    fclose(e);
    fclose(s);
    perror("Usage : -c/-d file_in file_out");
    return 1;
  }
  return 0;
}
