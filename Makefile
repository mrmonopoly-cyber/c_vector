C_FLAGS =  -fsanitizer=address -g

c_vector.o: c_vector.c
	gcc $(C_FLAGS) c_vector.c -c
