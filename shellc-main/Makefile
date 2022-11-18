CC=gcc
CFLAGS= -Wall -g
DEPS = head_list.h list_blocks.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

shell: head_list.o list_blocks.o main.o
	$(CC) -o shell head_list.o list_blocks.o main.o
