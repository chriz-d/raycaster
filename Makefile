CC := gcc
CPPFLAGS := -MMD -MP `sdl2-config --cflags`
CFLAGS := -Wall 
LDFLAGS := `sdl2-config --libs` -lm
EXE := raycaster
SRC_DIR := ./src
INC_DIR := ./include
OBJ_DIR := ./obj
SRC := $(wildcard $(SRC_DIR)/*.c)
INC := $(wildcard $(INC_DIR)/*.h)
OBJ := $(patsubst $(SRC_DIR)%,$(OBJ_DIR)%,$(patsubst %.c,%.o,$(wildcard $(SRC_DIR)/*.c)))
.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $@
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -I$(INC_DIR) $< -o $@

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

clean: 
	rm $(OBJ_DIR)/*.o
	rm $(OBJ_DIR)/*.d
	rm $(EXE)

-include $(OBJ:.o=.d)