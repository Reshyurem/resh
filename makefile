CC=gcc
CFLAGS=-I.
DEPS = header.h
OBJ = main.c linked_lists.c ls.c rawmode.c pinfo.c parse.c command_processing.c technical_functions.c shell_basics.c

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

resh: $(OBJ)
	$(CC) -g -o $@ $^ $(CFLAGS) 