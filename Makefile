CC = gcc
C_FLAGS = -Wall -Wextra
C_EXFLAGS = 

c_vector_src_path := $(or $(C_VECTOR_ROOT), .)
c_vector.c := $(c_vector_src_path)/c_vector.c

all: release

DEBUG_FLAGS = -O0 -fsanitize=address -g
RELEASE_FLAGS = -O2

debug: C_FLAGS += $(DEBUG_FLAGS)
debug: clean compile

release: C_FLAGS += $(RELEASE_FLAGS)
release: clean compile


compile: c_vector.o

c_vector.o: $(c_vector.c)
	$(CC) $(C_FLAGS) $(C_EXFLAGS) $(c_vector.c) -c

c_vector_clean:
ifeq ($(wildcard c_vector.o), c_vector.o)
	rm c_vector.o
endif


clean: c_vector_clean
