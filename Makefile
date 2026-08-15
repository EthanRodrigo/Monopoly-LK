CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Wunused-function -Iinclude \
		 -fsanitize=address,undefined

SRC = src/*.c
TARGET = game

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

debug:
	$(CC) $(CFLAGS) -g $(SRC) -o $(TARGET)
