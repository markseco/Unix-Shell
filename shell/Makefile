CC=gcc
CFLAGS= -Wall -g
DEPS = head_list.h list_blocks.h list_process.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

p3: head_list.o list_blocks.o list_process.o p3.o
	$(CC) -o p3 head_list.o list_blocks.o list_process.o p3.o
