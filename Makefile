CC = g++

all:
	$(CC) main.cpp -lglut -lassimp -lGLU -lGL -I. -o cgprog && ./cgprog

clean:
	rm cgprog
