#Makefile for WnV_the_game

#----------variables to control the makefile operation--------
CC = g++
TARGET = game
OBJECTS = Entities.o  Game.o  Grid.o  main.o

$(TARGET): $(OBJECTS)  constants.h 
	$(CC) -o $(TARGET) $(OBJECTS)

main.o:   main.cpp  constants.h
	$(CC) -c main.cpp

Entities.o: Entities.h constants.h  Entities.cpp
	$(CC) -c Entities.cpp

Game.o:	Game.h constants.h  Game.cpp
	$(CC) -c Game.cpp

Grid.o:	Grid.h constants.h Grid.cpp
	$(CC) -c Grid.cpp


#------------useful commands-----------
clean:
	rm -f $(TARGET) $(OBJECTS)

run:  
	./game 20 20