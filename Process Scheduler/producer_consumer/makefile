###################################
# CS 433 Programming Assignment 2
#  Preston Roberts, Kevyn Higbee
###################################
CC = g++ 					# use g++ for compiling c++ code or gcc for c code
CFLAGS = -std=c++11	-o3		# compilation flags: -std=c++11 for using the PROPER g++ library. Add -o or -o3 for optimized code.
LIB = -lm					# linked libraries	
LDFLAGS = -L. -lpthread			# link flags
PROG = Prog4			# target executable (output)
SRC = main.cpp PBuffer.cpp
OBJ = $(SRC:.cpp=.o) 		# object files for the target. Add more to this and next lines if there are more than one source files.

all : $(PROG)				

$(PROG): $(OBJ) 
	$(CC) -o $(PROG) $(OBJ) $(LDFLAGS) $(LIB)

.cpp.o:
	$(CC) -c $(CFLAGS) $< -o $@

# cleanup

clean:
	/bin/rm -f *.o $(PROG)

# DO NOT DELETE