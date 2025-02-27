CC = gcc
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
