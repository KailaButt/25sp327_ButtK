CC = gcc
<<<<<<< HEAD
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
=======
CFLAGS = -Wall -Wextra -g
TARGET = setuid_demo

all: $(TARGET)

$(TARGET): setuid_demo.c
	$(CC) $(CFLAGS) setuid_demo.c -o $(TARGET)

clean:
	rm -f $(TARGET)

test: $(TARGET)
	@echo "Setting up setuid-root permissions..."
	sudo chown root $(TARGET)
	sudo chmod u+s $(TARGET)
	ls -l $(TARGET)
	echo "Running the program as a non-root user..."
	./$(TARGET)
>>>>>>> 22d1b4de002239754401af3cca005efd85ff7a5a
