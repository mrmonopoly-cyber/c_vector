CSTD ?= c99
CSRC ?= main.c
ARGS ?= -pedantic-errors  -fno-builtin  -Wall -Wextra -fsanitize=address,undefined -g
INCLUDE ?= -I../..

build:
	gcc  -std=$(CSTD) $(ARGS) $(INCLUDE) $(CSRC) -o main

clean:
	rm main
