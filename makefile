# CC = gcc
# CFLAGS = -Wall -Wextra -std=c99 -pthread
# # c file name is main.c
# all: program

# program: main.o
# 	$(CC) $(CFLAGS) main.o -o program

# main.o: main.c
# 	$(CC) $(CFLAGS) -c main.c 
	
# run:
# 	./program

# clean:
# 	rm -f *.o program


CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pthread
LIBRARY = libvector.so 
# LIBRARYWINDOWS = libvector.dll
PROGRAM = program

all: $(PROGRAM)

$(PROGRAM): main.o $(LIBRARY)
	$(CC) $(CFLAGS) main.o -L. -lvector -o $(PROGRAM)

main.o: main.c vector.h
	$(CC) $(CFLAGS) -c main.c

$(LIBRARY): main.c vector.h
	$(CC) $(CFLAGS) -shared -o $(LIBRARY) main.c

run: $(PROGRAM)
	./$(PROGRAM)

clean:
	rm -f *.o $(PROGRAM) $(LIBRARY)
