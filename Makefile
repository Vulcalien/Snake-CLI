# Vulcalien's Executable Makefile
# version 0.3.2

# === Detect OS ===
ifeq ($(OS),Windows_NT)
    CURRENT_OS := WINDOWS
else
    CURRENT_OS := UNIX
endif
TARGET_OS := $(CURRENT_OS)

# === Basic Info ===
OUT_FILENAME := vulcalien-snake

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

SRC_SUBDIRS :=

# === Compilation ===
CPPFLAGS := -Iinclude -MMD -MP\
            -Ilib/libcliscreen/include
CFLAGS   := -Wall -pedantic

ifeq ($(TARGET_OS),UNIX)
    # UNIX
    CC := gcc

    CPPFLAGS +=
    CFLAGS   +=

    LDFLAGS := -Llib/libcliscreen/bin
    LDLIBS  := -lpthread -l:libcliscreen.a
else ifeq ($(TARGET_OS),WINDOWS)
    ifeq ($(CURRENT_OS),WINDOWS)
        # WINDOWS
        CC := gcc

        CPPFLAGS +=
        CFLAGS   +=

        LDFLAGS := -Llib/libcliscreen/bin
        LDLIBS  := -l:libwinpthread.a -l:libcliscreen.win.a
    else ifeq ($(CURRENT_OS),UNIX)
        # UNIX to WINDOWS cross-compile
        CC := x86_64-w64-mingw32-gcc

        CPPFLAGS +=
        CFLAGS   +=

        LDFLAGS := -Llib/libcliscreen/bin
        LDLIBS  := -l:libwinpthread.a -l:libcliscreen.win.a
    endif
endif

# === Extensions & Commands ===
ifeq ($(TARGET_OS),UNIX)
    OBJ_EXT    := o
    OUT_SUFFIX :=
else ifeq ($(TARGET_OS),WINDOWS)
    OBJ_EXT    := obj
    OUT_SUFFIX := .exe
endif

ifeq ($(CURRENT_OS),UNIX)
    MKDIR      := mkdir
    MKDIRFLAGS := -p

    RM      := rm
    RMFLAGS := -rfv
else ifeq ($(CURRENT_OS),WINDOWS)
    MKDIR      := mkdir
    MKDIRFLAGS :=

    RM      := rmdir
    RMFLAGS := /Q /S
endif

# === Resources ===

# list of source file extensions
SRC_EXT := c

# list of source directories
SRC_DIRS := $(SRC_DIR)\
            $(foreach SUBDIR,$(SRC_SUBDIRS),$(SRC_DIR)/$(SUBDIR))

# list of source files
SRC := $(foreach DIR,$(SRC_DIRS),\
         $(foreach EXT,$(SRC_EXT),\
           $(wildcard $(DIR)/*.$(EXT))))

# list of object directories
OBJ_DIRS := $(SRC_DIRS:%=$(OBJ_DIR)/%)

# list of object files
OBJ := $(SRC:%=$(OBJ_DIR)/%.$(OBJ_EXT))

# output file
OUT := $(BIN_DIR)/$(OUT_FILENAME)$(OUT_SUFFIX)

# === Targets ===

.PHONY: all run build build-dependencies clean

all: build-dependencies build run

run:
	./$(OUT)

build: $(OUT)

build-dependencies:
	$(MAKE) -C lib/libcliscreen build-static

clean:
	@$(RM) $(RMFLAGS) $(BIN_DIR) $(OBJ_DIR)
	$(MAKE) -C lib/libcliscreen clean

# generate output file
$(OUT): $(OBJ) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

# compile .c files
$(OBJ_DIR)/%.c.$(OBJ_EXT): %.c | $(OBJ_DIRS)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# create directories
$(BIN_DIR) $(OBJ_DIRS):
	$(MKDIR) $(MKDIRFLAGS) "$@"

-include $(OBJ:.$(OBJ_EXT)=.d)
