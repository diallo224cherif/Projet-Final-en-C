CC = gcc
CFLAGS = -Wall -std=c99 -g
INCLUDES = -Iinclude

# SEULEMENT les fichiers qui existent
SRC = src/main.c src/creatures.c
OBJ = $(SRC:.c=.o)
TARGET = oceandepths.exe

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

src/main.o: src/main.c include/creatures.h
	$(CC) $(CFLAGS) $(INCLUDES) -c src/main.c -o src/main.o

src/creatures.o: src/creatures.c include/creatures.h
	$(CC) $(CFLAGS) $(INCLUDES) -c src/creatures.c -o src/creatures.o

clean:
	del /Q src\*.o $(TARGET) 2>nul

run: $(TARGET)
	.\$(TARGET)

.PHONY: clean run