CC = gcc
CFLAGS = -I.
DEPS = word_index.h
OBJ = main.o word_index.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

word_index: $(OBJ)
	$(CC) -o  $@ $^ $(CFLAGS)
