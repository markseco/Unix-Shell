CC=gcc
CFLAGS= -Wall -g
DEPS = head_list.h list_blocks.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

p2: head_list.o list_blocks.o p2.o
	$(CC) -o p2 head_list.o list_blocks.o p2.o
