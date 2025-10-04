CC = gcc
CFLAGS = -Wall -std=c99 -g
INCLUDES = -Iinclude

SRC = src/main.c src/carte.c src/joueur.c src/creatures.c
OBJ = $(SRC:.c=.o)
TARGET = oceandepths.exe

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	del /Q src\main.o src\carte.o src\joueur.o src\creatures.o oceandepths.exe 2>nul

run: $(TARGET)
	.\$(TARGET)

.PHONY: clean run