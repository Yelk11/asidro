SRC_DIR := src
OBJ_DIR := obj


EXE := main
SRC := $(shell find $(SRC_DIR) -name '*.c')
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

LDFLAGS  := -lncurses
CFLAGS   := -Wall -g -Iinclude


.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	@$(RM) -rv $(OBJ_DIR)

-include $(OBJ:.o=.d)
