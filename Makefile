# JEAN Jordan, LEPAGE Tim, SERGEANT Dimitri, SYUHADA Fathin  --- Groupe 7 #
vpath %.h the_headers
vpath %.c the_sources

CC = gcc
CFLAGS= -Wall -Wextra -g -Iheaders -Isources
EXEC = lzw
SRCS = dict.c compression.c binio.c decompression.c
OBJS = $(SRCS:.c=.o)
all: $(EXEC) clean


lzw: lzw.o $(OBJS)
	$(CC) -o $(EXEC) lzw.o $(OBJS)


binio.o: sources/binio.c headers/binio.h headers/compression.h
	$(CC) $(CFLAGS) -c $<

dict.o: sources/dict.c headers/dict.h
	$(CC) $(CFLAGS) -c $<

compression.o: sources/compression.c headers/compression.h headers/dict.h headers/binio.h
	$(CC) $(CFLAGS) -c $<

decompression.o: sources/decompression.c headers/decompression.h headers/dict.h headers/binio.h
	$(CC) $(CFLAGS) -c $<

lzw.o: sources/lzw.c headers/decompression.h headers/dict.h headers/compression.h
	$(CC) $(CFLAGS) -c $<


clean:
	@rm -vrf *.o *.?~

mrproper: clean
	@rm -vrf $(EXEC)
