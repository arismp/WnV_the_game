#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <unistd.h>

#include "constants.h"
#include "Entities.h"
#include "Game.h"
#include "Grid.h"

int main(int argc,char *argv[]){

    //wrong input handling
    if(argc < 2){
        std::cout << "Wrong Input!!"<< std::endl << "Exepcted input is : ./game [integer] [integer]" << std::endl;
        return -1; 
    }

    //create a game and start playing 
    Game game;
    game.CreateGame(atoi(argv[1]),atoi(argv[2]));

    return 0;
}