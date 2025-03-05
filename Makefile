CC = gcc
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
