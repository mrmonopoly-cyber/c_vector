C_FLAGS =  -fsanitize=address -g

c_vector.o: c_vector.c
	gcc $(C_FLAGS) c_vector.c -c

clean:
	rm c_vector.o
