CC = gcc
CFLAGS = -Wall -Wextra

all: parent child

parent: parent.c
	$(CC) $(CFLAGS) -o parent parent.c

child: child.c
	$(CC) $(CFLAGS) -o child child.c

run: parent
	./parent

clean:
	rm -f parent child
