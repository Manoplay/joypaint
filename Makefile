all:
	gcc -lncurses src/main.c -o joypaint

clean:
	rm joypaint