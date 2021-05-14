ifeq ($(OS),Windows_NT)
  ifeq ($(shell uname -s),) # not in a bash-like shell
	CLEANUP = del /F /Q
	MKDIR = mkdir
  else # in a bash-like shell, like msys
	CLEANUP = rm -f
	MKDIR = mkdir -p
  endif
	TARGET_EXTENSION=.exe
else
	CLEANUP = rm -f
	MKDIR = mkdir -p
	TARGET_EXTENSION=.out
endif

C_COMPILER=gcc
ifeq ($(shell uname -s), Darwin)
C_COMPILER=clang
endif

UNITY_ROOT=../../UNITY

CFLAGS=-std=c11
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Wpointer-arith
CFLAGS += -Wcast-align
CFLAGS += -Wwrite-strings
CFLAGS += -Wswitch-default
CFLAGS += -Wunreachable-code
CFLAGS += -Winit-self
CFLAGS += -Wmissing-field-initializers
CFLAGS += -Wno-unknown-pragmas
CFLAGS += -Wstrict-prototypes
CFLAGS += -Wundef
CFLAGS += -Wold-style-definition

TARGET_BASE = deque

TARGET = $(TARGET_BASE)$(TARGET_EXTENSION)
SRC_FILES=$(UNITY_ROOT)/src/unity.c src/deque.c test/testFunc.c test/testFuncRunner.c
INC_DIRS=-Isrc -I$(UNITY_ROOT)/src
SYMBOLS=

all: clean test/testFuncRunner.c default

default: $(SRC_FILE)
	$(C_COMPILER) $(CFLAGS) $(INC_DIRS) $(SYMBOLS) $(SRC_FILES) -o $(TARGET)
	- ./$(TARGET)

test/testFuncRunner.c : test/testFunc.c
	ruby $(UNITY_ROOT)/auto/generate_test_runner.rb test/testFunc.c  test/testFuncRunner.c

clean:
	$(CLEANUP) $(TARGET)
ci: CFLAGS += -Werror
ci: default