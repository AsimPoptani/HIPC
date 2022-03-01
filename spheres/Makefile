.PHONY: all run generate clean

CC=gcc
FLAGS= -lpapi -lm -fopenmp

all: clean generate run

run:
	@echo "Running spheres..."
	@./spheres

generate:
	$(CC) $(FLAGS) -o spheres spheres.c

clean:
	@echo "Cleaning spheres..."
	@rm spheres