# === Paths ===
SRC_DIR := src
OBJ_DIR := $(SRC_DIR)/obj
BIN_DIR := bin
INC_DIR := include

# === Source Files ===
LIB_SRC := $(SRC_DIR)/mapextract.c
MAIN_SRC := $(SRC_DIR)/main.c
EXEC_SRCS := $(MAIN_SRC) $(LIB_SRC)

# === Object Files ===
LIB_OBJ := $(OBJ_DIR)/mapextract.o
EXEC_OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(EXEC_SRCS))

# === Targets ===
LIB := $(BIN_DIR)/libmapextract.so
EXEC := $(BIN_DIR)/mapextract

# === Compiler & Flags ===
CC := gcc
DEBUG ?= 0

# Base flags
BASE_CFLAGS := -Wall -Wextra -I$(INC_DIR)

ifeq ($(DEBUG), 1)
    BASE_CFLAGS += -g -O0 -DDEBUG
else
    BASE_CFLAGS += -O2 -DNDEBUG
endif

# Add -fPIC only when building shared lib
LIB_CFLAGS := $(BASE_CFLAGS) -fPIC
EXEC_CFLAGS := $(BASE_CFLAGS)

# === Build Targets ===
.PHONY: lib exec clean debug

all: exec

debug:
	$(MAKE) DEBUG=1

# === Compile Object Files ===
$(OBJ_DIR)/mapextract.o: $(SRC_DIR)/mapextract.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(LIB_CFLAGS) -c $< -o $@

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(EXEC_CFLAGS) -c $< -o $@

# === Build Shared Library ===
lib: $(LIB)

$(LIB): $(LIB_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) -shared $(LIB_CFLAGS) -o $@ $<

# === Build Executable ===
exec: $(EXEC)

$(EXEC): $(EXEC_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(EXEC_CFLAGS) -o $@ $^

# === Clean ===
clean:
	rm -rf $(OBJ_DIR)/*.o $(BIN_DIR)/*
