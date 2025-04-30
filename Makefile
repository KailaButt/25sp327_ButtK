CC = gcc
<<<<<<< HEAD
CFLAGS = -Wall -Wextra
OBJ = compare.o


all: compare_static compare_dynamic

compare_static: $(OBJ)
	$(CC) -static -o compare_static $(OBJ)

compare_dynamic: $(OBJ)
	$(CC) -o compare_dynamic $(OBJ)

compare.o: compare.c
	$(CC) $(CFLAGS) -c compare.c

.PHONY: clean
clean:
	rm -fv *.o compare_static compare_dynamic

.PHONY: compare
compare:
	ls -lh compare_static compare_dynamic
	./compare_static
	./compare_dynamic
=======
CFLAGS = -Wall -Wextra -g
OBJS = worker_thread.o thread_pool.o main.o queue.o tasks.o

request_server: $(OBJS)
	$(CC) $(CFLAGS) -o request_server $(OBJS) -lpthread

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o request_server
>>>>>>> 1b71d74 (fixed)
