C_FLAGS =  -fsanitize=address -g

c_vector.c := $(or ./c_vector.c, $(C_VECTOR_ROOT)/c_vector.c)

c_vector.o: $(c_vector.c)
	gcc $(C_FLAGS) $(c_vector.c) -c

clean:
	rm c_vector.o
