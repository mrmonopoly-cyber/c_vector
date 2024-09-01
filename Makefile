CC = gcc
C_FLAGS =-Wall -Wextra
C_EXFLAGS =

DEBUG_FLAGS = -O0 -fsanitize=address -g
RELEASE_FLAGS = -O2

c_vector_src_path := $(or $(C_VECTOR_ROOT), .)
c_vector.c := $(c_vector_src_path)/c_vector.c

all: release

debug: C_FLAGS += $(DEBUG_FLAGS)
debug: clean c_vector.o

release: C_FLAGS += $(RELEASE_FLAGS)
release: clean c_vector.o

c_vector.o: $(c_vector.c)
	$(CC) $(C_FLAGS) $(C_EXFLAGS) $(c_vector.c) -c

c_vector_clean:
ifeq ($(wildcard c_vector.o), c_vector.o)
	rm c_vector.o
endif


clean: c_vector_clean
