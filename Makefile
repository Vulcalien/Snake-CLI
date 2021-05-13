# Vulcalien's Executable Makefile
# version 0.1.1 (modified)
#
# Supported systems:
# - Linux
# - Windows
#
# Linux to Windows cross-compilation also supported

# ========= CONFIG =========
OUT_FILENAME := vulcalien-snake

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

CPPFLAGS := -Iinclude -MMD -MP
CFLAGS   := -Wall -pedantic

# Unix LDFLAGS and LDLIBS
UNI_LDFLAGS := -Llib
UNI_LDLIBS  := -lpthread

# Windows LDFLAGS and LDLIBS
WIN_LDFLAGS := -Llib
WIN_LDLIBS  := -l:libwinpthread.a

# ========= OS SPECIFIC =========
UNI_OBJ_EXT := .o
UNI_OUT_EXT :=

WIN_OBJ_EXT := .obj
WIN_OUT_EXT := .exe

ifeq ($(OS),Windows_NT)
	CC      := gcc
	OBJ_EXT := $(WIN_OBJ_EXT)
	OUT_EXT := $(WIN_OUT_EXT)

	LDFLAGS  := $(WIN_LDFLAGS)
	LDLIBS   := $(WIN_LDLIBS)

	RM      := del
	RMFLAGS := /Q
else
	CC      := gcc
	OBJ_EXT := $(UNI_OBJ_EXT)
	OUT_EXT := $(UNI_OUT_EXT)

	LDFLAGS  := $(UNI_LDFLAGS)
	LDLIBS   := $(UNI_LDLIBS)

	RM      := rm
	RMFLAGS := -rfv
endif

# ========= OTHER =========
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%$(OBJ_EXT))
OUT := $(BIN_DIR)/$(OUT_FILENAME)$(OUT_EXT)

.PHONY: all run build clean linux-to-windows

all: build run

run:
	./$(OUT)

build: $(OUT)

$(OUT): $(OBJ) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%$(OBJ_EXT): $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(DEBUGFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir $@

clean:
	@$(RM) $(RMFLAGS) $(BIN_DIR) $(OBJ_DIR)

linux-to-windows:
	make build CC=x86_64-w64-mingw32-gcc OBJ_EXT=$(WIN_OBJ_EXT) OUT_EXT=$(WIN_OUT_EXT) LDFLAGS=$(WIN_LDFLAGS) LDLIBS="$(WIN_LDLIBS)"

-include $(OBJ:$(OBJ_EXT)=.d)
