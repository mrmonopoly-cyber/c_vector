C_FLAGS =-Wall -Wextra  -fsanitize=address -g -fPIC

c_vector_src_path := $(or $(C_VECTOR_ROOT), .)
c_vector.c := $(c_vector_src_path)/c_vector.c

ifndef $(C_CHECK_INPUT_ROOT)
C_CHECK_INPUT_ROOT := $(c_vector_src_path)/lib/c_input_check
include $(C_CHECK_INPUT_ROOT)/Makefile
endif


all: c_vector.o

c_vector.o: $(c_vector.c)
	gcc $(C_FLAGS) $(c_vector.c) -c

c_vector_clean:
	rm c_vector.o


clean: c_vector_clean
