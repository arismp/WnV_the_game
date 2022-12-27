//file Grid.h contains the declarations of the Position class and the Grid class

#ifndef Grid_H
#define Grid_H

// #include "Entities.h"
// #include "Game.h"
//this is a class of a single position in the grid/map
//as its private member it has the coordinates of that position 
class Position{
    private:
        struct Cord{
            int x;
            int y;
        };
        struct Cord *cord;
    public:
        Position();
        struct Cord * GetPosition();
        void SetNewPosition(int newx,int newy);

};

//this is a grid class that creates,handles,updates etc the grid(map) of the game
class Grid{

    private:
        int x; //height
        int y; //width
        char **grid; //the actual grid/map of the game 
        char Day_n_Night_Cycle; // 'N' if it is night and 'D' if it is day
        int Max_Tree_Number;  //the number of trees that will be placed in the grid 
        int Max_Water_Number; //the number of water puddles that will be placed in the grid 
        int Cycle_count; //cycle count that counts the frames
        int EntitiesNum; //the number of vampires/werewolves in the grid 

    public:
        Grid();
        Grid(int X,int Y);
        void Init();
        ~Grid();
        char GetTime();
        void ShowDayAndNightCycle();
        void ShowGrid();
        void IncreaseCycleCount();
        void ChangeCycle();
        bool CheckPlace(int x,int y);
        void PlaceTrees();
        void PlaceWater();
        void PotionGenerator();
        char AccessGridPosition(int x,int y);
        int getX();
        int getY();
        int TotalEntitiesNum();
        void UpdateGrid(int x,int y,char Ch);
        /////////////////////////////////  bool isLand(Position *pos);
        bool isPotion(int x,int y);
};

#endif