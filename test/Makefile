C_FLAGS= -Wall -Wextra -g -fsanitize=address

C_VECTOR_ROOT = ..

include $(C_VECTOR_ROOT)/Makefile

OBJ_FILE = c_vector.o

all: main

main: main.c $(OBJ_FILE)
	gcc $(C_FLAGS) main.c $(OBJ_FILE) -o main

clean:
	rm main
