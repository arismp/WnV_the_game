#include <iostream>
#include <unistd.h>

#include "constants.h"
#include "Entities.h"
#include "Game.h"


//function that creates the word/map
Grid Game::CreateWorld(int x,int y){
    Grid grd1(x,y);
    grd1.Init(); //initialize the grid 
    grd1.PlaceTrees(); //generate random trees in the map(grid)
    grd1.PlaceWater(); //generate random rivers in the map(grid)
    grd1.PotionGenerator(); //generates a random potion in the map(grid)
    return grd1;
}


//this function waits for the player to press 'S' to start the gameplay 
void Game::Start(){
    std::cout << "To start the game press S..." << std::endl;
    char in;
    std::cin >> in;
    while(in != 'S'){
        std::cout <<"Need to press 'S'!" << std::endl;
        std::cin >> in;
    }    
}

//function that displays the game stats when the game is paused
void Game::DisplayGameStats(Entities *ent,avatar *player){
    ent->UpdateCount();
    system("stty cooked");
    std::cout<<std::endl;
    std::cout<<"The number of active vampires is : "<<ent->GetVampCount()<<std::endl;
    std::cout<<"The number of active werewolves is : "<<ent->GetWolfCount()<<std::endl;
    std::cout<<"The number of potions the player has is : "<<player->GetPotions()<<std::endl;
    return;    
}



//function that checks if a team has lost 
//if the team has lost then update the gameplay flag to false
bool Game::GameOver(Entities *ent){
    ent->UpdateCount();
    if(ent->GetVampCount() == 0){
        std::cout << "The werewolves have won!!" << std::endl;
        this->flag = false;
        return true;
    }
    else if(ent->GetWolfCount() == 0){
        std::cout << "The vampires have won!!" << std::endl;
        this->flag = false;
        return true; 
    }
    return false;
}


//this function waits for the player to press 'R' to resume the gameplay
void Game::Resume(avatar *player){
    while(player->PlayerInput()!= 'R') std::cout << "Need to press 'R' to resume the game!" << std::endl; 
}

//this function displays the game stats when the game is paused
void Game::PausedGame(Entities *en,avatar * player){
    DisplayGameStats(en,player); 
    std::cout << std::endl;   
}

//this function handles the actual gameplay 
//it has a while loop that each time the player gives a specific input 
//before the player the entities move and after the entities have moved they attack/heal teammate/dodge
void Game::GamePlay(Grid *gptr,avatar *player,Entities * ent){ 
    

    this->Start(); //start the game when the player presses 'S'
    gptr->ShowGrid();


    char input;
    //srand(time(NULL));

    //loop while the player presses 'Q' or a team has lost
    while(this->flag){

        //the entities move (vampires/werewolves)
        //srand(time(NULL));
        ent->EntitiesMovement(gptr);
        
        gptr->ChangeCycle();   //this function changes the day and night cycle

        system("stty raw");   //set the terminal to raw mode to get input without having to press enter after 
        input = player->PlayerInput(); 

        if(GameOver(ent)){ //if true that means a team has won
            this->flag = false;
            gptr->ShowGrid();
            continue;
        }
 
        if(input == 'Q'){
        //if the player's input is Q then the game ends
            this->flag=false;
            gptr->ShowGrid();
            continue;
        }
        else if(input == 'P'){   
        //if the player's input is P then pause the game and show the game stats until the player presses 'R'
            this->PausedGame(ent,player);
            this->Resume(player);
        }
        else if(input == 'H'){
        //if the player's input is 'H' then heal the whole team 
            player->Heal_Team(ent,gptr);
        }
        else{
            //first the player moves 
            player->PlayerMovement(input,gptr);
            //increase the cycle count after each frame
            gptr->IncreaseCycleCount();
            gptr->ShowGrid();
            //usleep(50000);
        }

    }
    std::cout << "***GAME-OVER***" << std::endl;
}

void Game::CreateGame(int x,int y){

    //create the world 
    Grid grid = CreateWorld(x,y); ///////////////////////////////

    //create the player and place them in a random position in the grid/world
    avatar Player;
    Player.StartingPoint(&grid);
    
    //create all the vampires and the werewolves of the game 
    Entities entities;
    entities.CreateEntities(&grid);

    //start the Game
    this->GamePlay(&grid,&Player,&entities);
    system("stty cooked");

}
