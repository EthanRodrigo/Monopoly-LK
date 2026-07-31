CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Iinclude 

SRC = src/board.c
TARGET = game

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)
