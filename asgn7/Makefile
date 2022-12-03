SOURCES  = $(wildcard *.c)
OBJECTS  = $(SOURCES:%.c=%.o)

CC       = clang
CFLAGS   = -Wall -Wpedantic -Werror -Wextra -Ofast -gdwarf-4 -g

.PHONY: all clean spotless format

all: encode decode

encode: encode.o code.o io.o huffman.o node.o stack.o pq.o
	$(CC) -o $@ $^

decode: decode.o code.o io.o huffman.o node.o stack.o pq.o
	$(CC) -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o

format:
	clang-format -i -style=file *.[ch]

spotless:
	rm -f encode decode *.o