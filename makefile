test: test.o read.o print.o
	gcc test.o read.o print.o

test.o:	test.c
	gcc -c test.c

read.o: read.c
	gcc -c read.c

print.o: print.c
	gcc -c print.c

clean:
	rm -f *.o