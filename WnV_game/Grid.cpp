#include <iostream>

#include "Grid.h"
#include "constants.h"

//this is the implementation of Position's methods
Position::Position(){ 
    this->cord = new Cord();
    this->cord->x = -1;
    this->cord->y = -1; 
}
Position::~Position(){
    delete this->cord;
}

struct Position::Cord *Position::GetPosition(){ return this->cord; }

void Position::SetNewPosition(int newx,int newy){
    this->cord->x=newx;
    this->cord->y=newy;
}



//this is the implementation of Grid's methods
Grid::Grid(){ this->x=-1,this->y=-1; } //initialize x and y to -1
Grid::Grid(int X,int Y){       //construct the grid 
    this->x=X,this->y=Y;
    this->Max_Tree_Number = (this->x*this->y)/40;    //the number of water puddles and trees is equal to (x*y)/40  
    this->Max_Water_Number = (this->x*this->y )/40; 
    int startingtime = (rand() % 2) + 1; //radnomly pick if the game will start at day or at night
    if(startingtime == 1) this->Day_n_Night_Cycle = 'N';   
    else this->Day_n_Night_Cycle = 'D';      
    this->Cycle_count = 0;  
    this->EntitiesNum = (this->x * this->y)/15; //the number of vampires/werewolves is equal to (x*y)/15
}
void Grid::Init(){   //initialize the grid and allocate memory for it 
    int i;
    this->grid=new char *[(this->x)];
    for(i=0;i<this->x;i++){
        this->grid[i] = new char [this->y];
    }
    for(i=0;i<this->x;i++){   //make every tile/position on the grid = '.' (land = '.')
        for(int j=0;j<this->y;j++){
            this->grid[i][j]='.';
        }
    }
}
Grid::~Grid(){ delete[] this->grid; } //delete the grid 

char Grid::GetTime(){ return this->Day_n_Night_Cycle; }

void Grid::ShowDayAndNightCycle(){ // show when is day and when is night 
    std::cout << std::endl;
    std::cout << " ";
    if(this->Day_n_Night_Cycle == 'D') std::cout << "DAY";
    else std::cout << "NIGHT";
    std::cout << std::endl;
}

void Grid::ShowGrid(){  //print the grid 
    system("stty cooked"); //set the terminal to cooked mode
    ShowDayAndNightCycle(); //above the grid print the day and night cycle 
    int c;
    for(c=0;c<this->y;c++){
        if(c == 0) std::cout << " "; 
        std::cout << "__"; 
    }
    std::cout << "_"; 
    std::cout << std::endl;
    for(int i=0;i<this->x;i++){
        for(int j=0;j<this->y;j++){
            if(j == 0) std::cout << "| ";
            std::cout << this->grid[i][j] << " ";
            if(j == this->y - 1) std::cout << "|";
        }
        std::cout << std::endl;
    }
    
    for(c=0;c<this->y;c++) {
        if(c == 0) std::cout << " ";
        std::cout << "--";
    } 
    std::cout << "-";
    std::cout << std::endl;
}

void Grid::IncreaseCycleCount(){ this->Cycle_count++; } //increase the cycle count (after the player's movement)

void Grid::ChangeCycle(){
//this function changes the day and night cycle based on the cycle count(frames)
    if(this->Cycle_count == 20){
        if(this->Day_n_Night_Cycle == 'N') this->Day_n_Night_Cycle = 'D';
        else this->Day_n_Night_Cycle = 'N';
        this->Cycle_count = 0;
    }
}

bool Grid::CheckPlace(int x,int y){ //check if a position is land
    if(this->grid[x][y]=='.') return true;
    return false;
}


void Grid::PlaceTrees(){   //place trees randomly in the grid (loop while a random grid position is empty)
    srand(time(NULL));
    for(int i=0;i<this->Max_Tree_Number;i++){
        int randX,randY;
        do{
            randX=rand()%this->x;
            randY=rand()%this->y;
        }while(CheckPlace(randX,randY)==false);
        this->grid[randX][randY]=TREE;
    }
}

void Grid::PlaceWater(){    //place water puddles randomly in the grid (loop while a random grid position is empty)
    srand(time(NULL));
    for(int i=0;i<this->Max_Water_Number;i++){  
        int randX,randY;             
        do{
            randX=rand()%this->x;
            randY=rand()%this->y;
        }while(CheckPlace(randX,randY)==false); 
        this->grid[randX][randY]=WATER;    
    }
}

void Grid::PotionGenerator(){ //this function places a potion in a random place in the grid(loop while a random grid position is empty)
    srand(time(NULL));
    int randX,randY;             
    do{
        randX=rand()%this->x;
        randY=rand()%this->y;
    }while(CheckPlace(randX,randY)==false);
    this->grid[randX][randY] = 'p';
}

char Grid::AccessGridPosition(int x,int y){ return this->grid[x][y]; }  //access a grid's position based on the given coordinates(x,y)
int Grid::getX(){ return this->x; }   //get x(height)
int Grid::getY(){ return this->y; }   //get y(width)
int Grid::TotalEntitiesNum(){ return this->EntitiesNum; } //get the entities number
void Grid::UpdateGrid(int x,int y,char Ch){ this->grid[x][y]=Ch; }  //update a position on the grid 


bool Grid::isPotion(int x,int y){ //this function checks if a gird's position is a potion or not(based on the given coordiantes)
    if(this->grid[x][y] == 'p') return true;
    return false;
}