CC=gcc
TARGET=reverse_shell

all:
	$(CC) reverse_shell.c -o $(TARGET)
	strip --strip-all $(TARGET)

clean:
	rm $(TARGET)