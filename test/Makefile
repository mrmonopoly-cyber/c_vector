CC = gcc
C_FLAGS = -Wall -Wextra
C_EXFLAGS = 

C_VECTOR_ROOT = ..
OBJ_FILE = c_vector.o

include $(C_VECTOR_ROOT)/Makefile

all: release

DEBUG_FLAGS = -O0 -fsanitize=address -g
RELEASE_FLAGS = -O2

debug: C_FLAGS += $(DEBUG_FLAGS)
debug: clean compile

release: C_FLAGS += $(RELEASE_FLAGS)
release: clean compile


compile: main

main: main.c $(OBJ_FILE)
	gcc $(C_FLAGS) main.c $(OBJ_FILE) -o main

clean:
ifeq ($(wildcard main) , main)
	rm main
endif

