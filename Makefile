CC= gcc
C_FLAGS =-Wall -Wextra  -fsanitize=address -g

c_vector_src_path := $(or $(C_VECTOR_ROOT), .)
c_vector.c := $(c_vector_src_path)/c_vector.c

all: c_vector.o

c_vector.o: $(c_vector.c)
	$(CC) $(C_FLAGS) $(c_vector.c) -c

c_vector_clean:
	rm c_vector.o


clean: c_vector_clean
