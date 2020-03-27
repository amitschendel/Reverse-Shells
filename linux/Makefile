CC=gcc
TARGET=reverse_shell

all:
	$(CC) reverse_shell.c -o $(TARGET)
	strip -F $(TARGET) --strip-all

clean:
	rm $(TARGET)