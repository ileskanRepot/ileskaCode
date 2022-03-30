CC=gcc
FLAGS=-g

hellomake: main.c
	 $(CC) $(FLAGS) main.c -o main
