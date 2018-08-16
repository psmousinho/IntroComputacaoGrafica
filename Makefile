CC = g++

all:
	$(CC) main.cpp -lglut -lGLU -lGL -I. -o cgprog && ./cgprog

clean:
	rm cgprog

