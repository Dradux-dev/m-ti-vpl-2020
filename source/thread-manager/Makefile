CC = g++
CFLAGS = -pthread
SRC = main.cpp threadManager.cpp
OBJ = main.o threadManager.o
PROG = prog

$(PROG): $(OBJ)
	$(CC) -o $(PROG) $(OBJ) $(CFLAGS)

%.o: %.cpp
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o prog

