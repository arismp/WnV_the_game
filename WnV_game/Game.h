//this file contains the declarations of the Game class, the Statistics class plus the prototype of CreateWorld

#ifndef Game_H
#define Game_H

#include "constants.h"


//the game class that controls the actual game 
class Game{
    private:
        bool flag; //flag that handles the gameplay loop 
    public:
        Game();
        ~Game();
        Grid CreateWorld(int x,int y);
        bool GameOver(Entities *ent);
        void CreateGame(int x,int y);
        void Start();
        void PausedGame(Entities *en,avatar * player);
        void Resume(avatar *player);
        void GamePlay(Grid *gptr,avatar *player,Entities * ent);
        void DisplayGameStats(Entities *ent,avatar * player);
};
#endif