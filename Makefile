all:
	gcc  src/main.c -lncurses -o joypaint

clean:
	rm joypaint
