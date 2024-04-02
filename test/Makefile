FLAGS= -Wall -Wextra -g -fsanitize=address

all: main

main: c_vector.o c_vector.h
	gcc $(FLAGS) main.c c_vector.o -o main

clean:
	rm main
