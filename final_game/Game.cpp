#include <iostream>
#include <unistd.h>

#include "constants.h"
// #include "Entities.h"
#include "Game.h"
// #include "Grid.h"


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


//this function waits for the player to press 'R' to resume the gameplay
void Game::Resume(avatar *player){
    while(player->PlayerInput()!= 'R') std::cout << "Need to press 'R' to resume the game!" << std::endl; 
}

//this function displays the game stats when the game is paused
void Game::Pause(Entities *en,Statistics *stats,avatar * player){
    char input;
    stats->CountVampires(en);
    stats->CountWerewolves(en);
    stats->CountPotions(player->GetPotions()); 
    std::cout << std::endl;
    stats->ShowStats();    
}

//this function handles the actual gameplay 
//it has a while loop that each time the player gives a specific input 
//and after his/hers action the entites move
//after the entities have moved they attack/heal teamate/dodge
void Game::GamePlay(Grid *gptr,avatar *player,Entities * ent){

    this->flag=true;  
    
    Statistics stats;

    this->Start(); //start the game when the player presses 'S'
    gptr->ShowGrid();
    char input;

    //loop while the player presses 'Q' or a team has lost
    while(this->flag){
        
        gptr->ChangeCycle();   //this function changes the day and night cycle

        system("stty raw");   //set the terminal to raw mode to get input without having to press enter after 
        input = player->PlayerInput(); 
 
        if(input == 'Q'){
        //if the player's input is Q then the game ends
            this->flag=false;
            gptr->ShowGrid();
            std::cout << "***GAME-OVER***" << std::endl;
            continue;
        }
        else if(input == 'P'){    //add system("pause")
        //if the player's input is P then pause the game and show the game stats until the player presses 'R'
            this->Pause(ent,&stats,player);
            this->Resume(player);
        }
        else if(input == 'H'){
        //if the player's input is 'H' then heal the whole team 
            player->Heal_Team(ent,gptr);
        }
        else{
            //first the player moves 
            player->PlayerMovement(input,gptr);
            //after the player's movement the other entities move 
            srand(time(NULL));
            ent->EntitiesMovement(gptr);
            ent->EntitiesAction(gptr);
            //increase the cycle count after each frame
            gptr->IncreaseCycleCount();
            gptr->ShowGrid();
            //usleep(100000); 
            usleep(50000);

            // CheckGameStatus();
        }

    }
}
//this method checks the status of each team and determines if the game has ended
void Game::CheckGameStatus(Entities *ent){

    int vampNum = ent->GetVampCount();
    int wolfNum = ent->GetWolfCount();
    
    if(vampNum == 0){
        std::cout << "All vampires have been defeated." << std::endl;
        std::cout << "Werewolves win!!!" << std::endl; 
    }
    else if(wolfNum == 0){
        std::cout << "All werewolves have been defeated." << std::endl;
        std::cout << "Vampires win!!!" << std::endl; 
    }

}


void Game::CreateGame(int x,int y){

    //create the world 
    Grid grid = CreateWorld(x,y);
    grid.ShowGrid();

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

Grid CreateWorld(int x,int y){
    Grid grd1(x,y);
    grd1.Init(); //initialize the grid 
    grd1.PlaceTrees(); //generate random trees in the map(grid)
    grd1.PlaceWater(); //generate random rivers in the map(grid)
    grd1.PotionGenerator(); //generates a random potion in the map(grid)
    return grd1;
}



//this is the implementation of the Statistics' methods
Statistics::Statistics(){ //constructor 
    this->VampiresNum=0;
    this->WerewolvesNum=0;
    this->PotionsNum=0;
}
Statistics::~Statistics(){ } //destructor 

//this function counts the active vampires
//the number of active vampires is the size of the vampire vector(in the Entities class)
void Statistics::CountVampires(Entities *en){
    en->UpdateCount();
    this->VampiresNum = en->GetVampCount();
}

//this function counts the active werewolves
//the number of active werewolves is the size of the werewolf vector(in the Entities class)        
void Statistics::CountWerewolves(Entities *en){
    en->UpdateCount();
    this->WerewolvesNum = en->GetWolfCount();
} 

//this function counts the potions the player has 
//as a parameter it receives an int number that is the potions private member of the avatar class
void Statistics::CountPotions(int potions){ this->PotionsNum = potions; }

//get vampires,werewolves,potions number
int Statistics::getVampNum(){ return this->VampiresNum; }  
int Statistics::getWereNum(){ return this->WerewolvesNum; }
int Statistics::getPotionsNum(){ return this->PotionsNum; } 

//function that displays the game stats when the game is paused
void Statistics::ShowStats(){
    system("stty cooked");
    std::cout << "The number of active vampires is : " << this->getVampNum() << std::endl;
    std::cout << "The number of active werewolves is : " << this->getWereNum() << std::endl;
    std::cout << "The number of potions the player has is : "<< this->getPotionsNum() << std::endl;
    return;
}      
