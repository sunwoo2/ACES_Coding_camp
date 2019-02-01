
run: all
	./test

all: lib
	gcc -o test test.c -lmath -L.

lib: sub.o sum.o
	ar rc libmath.a sub.o sum.o

test.o:
	gcc -c test.c

sub.o:
	gcc -c sub.c

sum.o:
	gcc -c sum.c

clean:
	rm -rf test *.o lib*.a
