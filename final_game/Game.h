//this file contains the declarations of the Game class, the Statistics class plus the prototype of CreateWorld

#ifndef Game_H
#define Game_H

#include "constants.h"


//function prototype
Grid CreateWorld(int x,int y);


//this class handles the game stats that will be displayed when the game is paused
class Statistics{
    private:
        int VampiresNum;  //number of active vampires 
        int WerewolvesNum; //number of active werewolves
        int PotionsNum; //number of potions that the player has 

    public:
        Statistics();
        ~Statistics();

        void CountVampires(Entities *en);
        void CountWerewolves(Entities *en);  
        void CountPotions(int potions);

        int getVampNum();
        int getWereNum();
        int getPotionsNum();

        void ShowStats();
};


//the game class that controls the actual game 
class Game{
    private:
        bool flag; //flag that handles the gameplay loop 
    public:
        Game(){ this->flag = false; }
        void CreateGame(int x,int y);
        void Start();
        void Pause(Entities *en,Statistics *stats,avatar * player);
        void Resume(avatar *player);
        void GamePlay(Grid *gptr,avatar *player,Entities * ent);
};
#endif