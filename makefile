test: test.o read.o print.o
	gcc -g test.o read.o print.o

test.o:	test.c
	gcc -g -c test.c

read.o: read.c
	gcc -g -c read.c

print.o: print.c
	gcc -g -c print.c

clean:
	rm -f *.o