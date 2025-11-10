# Makefile — OceanDepth (C99)

# Dossiers
SRC_DIR := src
INC_DIR := include
BIN_DIR := bin
OBJ_DIR := obj

# Binaire
TARGET  := $(BIN_DIR)/oceandepths

# Outils
CC      := gcc
CFLAGS  := -std=c99 -Wall -Wextra -Werror -I$(INC_DIR)
LDFLAGS := 

# Sources et objets
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

# Règle par défaut
all: prep $(TARGET)

# Link final
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

# Compilation unitaire
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Préparation des dossiers
prep:
	@mkdir -p $(BIN_DIR) $(OBJ_DIR)

# Nettoyage
clean:
	$(RM) -r $(OBJ_DIR)

fclean: clean
	$(RM) -r $(BIN_DIR)

re: fclean all

.PHONY: all prep clean fclean re