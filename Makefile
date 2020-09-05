CC := gcc
CPPFLAGS = -MMD -MP
CFLAGS := -Wall 
LDFLAGS :=
EXE := test
SRC_DIR := src
OBJ_DIR := obj
SRC := $(wildcard $(SRC_DIR)/*.c))
OBJ := $(patsubst $(SRC_DIR)%,$(OBJ_DIR)%,$(patsubst %.c,%.o,$(wildcard $(SRC_DIR)/*.c)))
.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $@
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

clean: 
	rm $(OBJ_DIR)/*.o
	rm $(OBJ_DIR)/*.d
	rm $(EXE)

-include $(OBJ:.o=.d)