#ifndef BINIO_H_
#define BINIO_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

typedef enum {
        BINIO_OK = 0,
        BINIO_EOF = -1,
        BINIO_BADESBIN = -2,
        BINIO_MAXESBIN = -3,
        BINIO_BADVALUE = -4,
        BINIO_IOERROR = -5
} binio_error_t;

binio_error_t binio_write(FILE *f, int value, unsigned int bitsize);

binio_error_t binio_read (FILE *f, int *value, unsigned int bitsize);

#endif  // BINIO_H_
