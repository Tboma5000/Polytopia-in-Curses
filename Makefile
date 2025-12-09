CC=gcc
CFLAGS=-std=c11 -Wall -Wno-vla-parameter
LDLIBS=-lm -lncursesw
OUTPUT=game

all: $(OUTPUT)

$(OUTPUT): mechanics.o main.o gui.o
#	cppcheck --enable=performance,unusedFunction --error-exitcode=1 * .c
	$(CC) $(CFLAGS) mechanics.o main.o gui.o $(LDLIBS) -o $(OUTPUT)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c $(LDLIBS) 
# -o main.o

mechanics.o: mechanics.c mechanics.h
	$(CC) $(CFLAGS) -c mechanics.c $(LDLIBS) 
# -o mechanics.o
gui.o: gui.c gui.h
	$(CC) $(CFLAGS) -c gui.c $(LDLIBS) 

clean:
	rm -rf *.o (OUTPUT)