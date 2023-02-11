TARGET = Chat
CC = g++
SRC = $(wildcard *.cpp)
OBJ = $(patsubst %.cpp, %.o, $(SRC))

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) 

%.o: %.cpp
	$(CC) -c $< -o $@
	
clear:
	rm -f *.o