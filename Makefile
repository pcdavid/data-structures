all: compile
	ar r datastructures.a *.o

compile: $(patsubst %.c,%.o,$(wildcard *.c))

clean:
	-rm -f *.o
	-rm -f datastructures.a
