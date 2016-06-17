all: surfers

surfers_test.o: surfers_test.c surfers.h
	gcc -c surfers_test.c

surfers.o: surfers.c surfers.h
	gcc -c surfers.c

surfers: surfers.o surfers_test.o
	gcc -o surfers surfers.o surfers_test.o -lpthread

clean: 
	rm -f surfers *.o
