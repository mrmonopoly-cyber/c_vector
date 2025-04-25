CC = gcc
C_FLAGS = -Wall -Wextra
C_EXFLAGS = 

c_vector_src_path := $(or $(C_VECTOR_ROOT), .)
c_vector.c := $(c_vector_src_path)/c_vector.c

all: release

DEBUG_FLAGS = -O0 -fsanitize=address -g
RELEASE_FLAGS = -O2

debug: C_FLAGS += $(DEBUG_FLAGS)
debug: static dynamic

release: C_FLAGS += $(RELEASE_FLAGS)
release: static dynamic

static_c_vector.o: $(c_vector.c)
	$(CC) $(C_FLAGS) $(C_EXFLAGS) $(c_vector.c) -c

dynamic_c_vector.so: $(c_vector.c)
	$(CC) $(C_EXFLAGS) $(DEBUG) $(c_vector.c) -fPIC -shared -o c_vector.so

static: static_c_vector.o

dynamic: dynamic_c_vector.so

c_vector_clean:
ifeq ($(wildcard c_vector.o), c_vector.o)
	rm c_vector.o
endif
ifeq ($(wildcard c_vector.so), c_vector.so)
	rm c_vector.so
endif


clean: c_vector_clean
