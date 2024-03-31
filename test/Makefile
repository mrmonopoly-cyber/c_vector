FLAGS= -Wall -Wextra -g -fsanitize=address

all: main

main: c_vector.c c_vector.h
	gcc $(FLAGS) main.c c_vector.c -o main

clean:
	rm main
