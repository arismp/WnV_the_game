#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <unistd.h>



using namespace std;



const char UP = 65;   //Up movement
const char DOWN = 66; //Down movement 
const char RIGHT = 67; //rigth movement
const char LEFT = 68; //left movement 

const int MAX_POWER = 3;  //maximum power is 3
const int MAX_HEALTH = 3; //maximum health is 3
const int MAX_DEFENSE = 3; //maximum defense is 3
const int MAX_POTIONS = 2; //maximum number of potions is 2 

const char WEREWOLF = 'w'; 
const char VAMPIRE = 'v';

const char WATER = '=';
const char TREE = 'T';



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
        Position(){ 
            this->cord = new Cord();
            this->cord->x = -1;
            this->cord->y = -1; 
        }
        struct Cord * GetPosition(){ return this->cord; }
        void SetNewPosition(int newx,int newy){
            this->cord->x=newx;
            this->cord->y=newy;
        }

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
        Grid(){ this->x=-1,this->y=-1; } //initialize x and y to -1
        Grid(int X,int Y){       //construct the grid 
            this->x=X,this->y=Y;
            this->Max_Tree_Number = (this->x*this->y)/40;    //the number of water puddles and trees is equal to (x*y)/40  
            this->Max_Water_Number = (this->x*this->y )/40; 
            this->Day_n_Night_Cycle = 'N';     
            this->Cycle_count = 0;  
            this->EntitiesNum = (this->x * this->y)/30; //the number of vampires/werewolves is equal to (x*y)/30
        }
        void Init(){   //initialize the grid and allocate memory for it 
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
        ~Grid(){ delete[] this->grid; } //delete the grid 

        char GetTime(){ return this->Day_n_Night_Cycle; }

        void ShowDayAndNightCycle(){ // show when is day and when is night 
            cout << endl;
            cout << " ";
            if(this->Day_n_Night_Cycle == 'D') cout << "DAY";
            else cout << "NIGHT";
            cout << endl;
        }
        
        void ShowGrid(){  //print the grid 
            system("stty cooked"); //set the terminal to cooked mode
            ShowDayAndNightCycle(); //above the grid print the day and night cycle 
            int c;
            for(c=0;c<this->y;c++){
                if(c == 0) cout<<" "; 
                cout<<"__"; 
            }
            cout<<"_"; 
            cout<<endl;
            for(int i=0;i<this->x;i++){
                for(int j=0;j<this->y;j++){
                    if(j == 0) cout<<"| ";
                    cout<<this->grid[i][j]<<" ";
                    if(j == this->y - 1) cout<<"|";
                }
                cout<<endl;
            }
            
            for(c=0;c<this->y;c++) {
                if(c == 0) cout<<" ";
                cout<<"--";
            } 
            cout<<"-";
            cout<<endl;
        }

        void IncreaseCycleCount(){ this->Cycle_count++; } //increase the cycle count (after the player's movement)

        void ChangeCycle(){
        //this function changes the day and night cycle based on the cycle count(frames)
            if(this->Cycle_count == 100){
                if(this->Day_n_Night_Cycle == 'N') this->Day_n_Night_Cycle = 'D';
                else this->Day_n_Night_Cycle = 'N';
                this->Cycle_count = 0;
            }
        }

        bool CheckPlace(int x,int y){ //check if a position is land
        //(the only difference with the isLand() function is that this function takes x,y coordinates as an argument)
            if(this->grid[x][y]=='.') return true;
            return false;
        }


        void PlaceTrees(){   //place trees randomly in the grid (loop while a random grid position is empty)
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

        void PlaceWater(){    //place water puddles randomly in the grid (loop while a random grid position is empty)
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

        void PotionGenerator(){ //this function places a potion in a random place in the grid(loop while a random grid position is empty)
            srand(time(NULL));
            int randX,randY;             
            do{
                randX=rand()%this->x;
                randY=rand()%this->y;
            }while(CheckPlace(randX,randY)==false);
            this->grid[randX][randY] = 'p';
        }

        char AccessGridPosition(int x,int y){ return this->grid[x][y]; }  //access a grid's position based on the given coordinates(x,y)
        int getX(){ return this->x; }   //get x(height)
        int getY(){ return this->y; }   //get y(width)
        int TotalEntitiesNum(){ return this->EntitiesNum; } //get the entities number
        void UpdateGrid(int x,int y,char Ch){ this->grid[x][y]=Ch; }  //update a position on the grid 
        bool isLand(Position *pos){ //checks wether a position on the grid is land
            if(this->grid[pos->GetPosition()->x][pos->GetPosition()->y]=='.') return true;
            return false;
        }
        bool isPotion(int x,int y){ //this function checks if a gird's position is a potion or not(based on the given coordiantes)
            if(this->grid[x][y] == 'p') return true;
            return false;
        }
        
};




//maybe a virtual function 
//also has the position of the object on the grid 
class character{

    private:
        char Species; //'v' for vmapires 'w', for werewolves and 'V'/'W' for the avatar depending on the team they support
        Position *pos;

    public:
        character(){ //constructor 
            this->pos = new Position ();
            this->Species = '\0';   //it is initialized to '\0'
        }

        void SetSpecies(char S){ this->Species = S; }
        char GetSpecies(){ return this->Species; } 

        //this function updates the entity's position after each movement
        void UpdatePosition(int x,int y){ this->pos->SetNewPosition(x,y); } 
        
        //this function places the entity in a random position on the grid (loop while it finds a position that is free(land))
        void StartingPoint(Grid *grid){ 
            srand(time(NULL));
            int randX,randY;
            do{
                randX = rand() % grid->getX();
                randY = rand() % grid->getY();
            }while(!(grid->CheckPlace(randX,randY)));
            grid->UpdateGrid(randX,randY,this->GetSpecies());
            UpdatePosition(randX,randY);  
        }

        //access to the entity's position 
        Position *GetEntityPosition(){ return this->pos; }  

        //this function checks if an entity is in the grid's boundary by checking its position's coordinates
        bool isInBoundary(Grid *grid){
            //int x = this->GetEntityPosition()->GetPosition()->x;
            //int y = this->GetEntityPosition()->GetPosition()->y;
            int x = this->pos->GetPosition()->x;
            int y = this->pos->GetPosition()->y;
            if(x == 0 || y == 0 || x == grid->getX()-1 || y == grid->getY()-1) return true;
            return false;
        }                  


};


//this class inherits the public part of the character class
//the vampire and werewolf classes will inherit the public part of this class 
//it contains the normal movement a werewolf/vampire can do (up,down,left,right)
//it also contains information such as the entity's health,power,defense
class Creature: public character{
    private:
        int health;
        int power;
        int defense;
        int potions;
       
    public:
        Creature(){ //constructor
            this->health = MAX_HEALTH; //every entity starts with a full health bar 
            srand(time(NULL));
            this->power = (rand()%MAX_POWER)+1; //every entity is given a random power from 1-MAX_POWER
            this->defense = (rand()%MAX_DEFENSE)+1; //every entity is given a random defense from 1-MAX_DEFENSE
        }   

        //set the new entity's health status 
        void SetHealth(int h){ this->health =h; }

        //get the entity's health,power,defense and potions status
        int GetHealthState(){ return this->health; }
        int GetPower(){ return this->power; }
        int GetDefense(){ return this->defense; }             
        int GetPotions(){ return this->potions; }

        //decrease the entity's potions by 1
        void DecreasePotions(){ this->potions--; }

        //this function check if a movement is legal (returns true if legal and false if not)
        //it checks whether an entity is in the grid's boundary so it does't go out of bounds
        //it also checks if the next position is land (which means it can move) 
        bool isLegalMovement(char mov,Grid *grid){
            bool flag = true;
            switch(mov){
                case 1: //UP
                    if(this->isInBoundary(grid) && this->GetEntityPosition()->GetPosition()->x == 0 ){
                        flag = false;
                        break;
                    }
                    if(!grid->CheckPlace(this->GetEntityPosition()->GetPosition()->x-1,this->GetEntityPosition()->GetPosition()->y)) flag = false;                    
                    break;
                case 2: //DOWN
                    if(this->isInBoundary(grid) && this->GetEntityPosition()->GetPosition()->x == grid->getX()-1){
                        flag = false;
                        break;
                    }
                    if(!grid->CheckPlace(this->GetEntityPosition()->GetPosition()->x+1,this->GetEntityPosition()->GetPosition()->y)) flag = false;                
                    break;
                case 3: //RIGHT
                     if(this->isInBoundary(grid) && this->GetEntityPosition()->GetPosition()->y == grid->getY()-1){
                        flag = false;
                        break;
                    }
                    if(!grid->CheckPlace(this->GetEntityPosition()->GetPosition()->x,this->GetEntityPosition()->GetPosition()->y+1)) flag = false;               
                    break;
                case 4: //LEFT
                    if(this->isInBoundary(grid) && this->GetEntityPosition()->GetPosition()->y == 0){
                        flag = false;
                        break;
                    }
                    if(!grid->CheckPlace(this->GetEntityPosition()->GetPosition()->x,this->GetEntityPosition()->GetPosition()->y-1)) flag = false;                
                    break;
            }
            return flag;
        }



        int Pick_Random_Movement(){ 
        //this function picks a random vampire movement 
        //from 1-4 : 1 = up,2 = down,3 = rigth,4 = left
            return (rand() % 4)+1;
        }        

        //this function handles the entity's movement 
        //if the random movement the entity wants to make is legal then it updates the previous grid position to be land
        //and the next position to be the entity's symbol
        //after each movement the entity's position must also be updated
        void Movement(Grid *grid){
            int movement = Pick_Random_Movement();
            switch(movement){
                case 1: //move UP
                    if(isLegalMovement(movement,grid)){
                        grid->UpdateGrid(this->GetEntityPosition()->GetPosition()->x,this->GetEntityPosition()->GetPosition()->y,'.');
                        grid->UpdateGrid(this->GetEntityPosition()->GetPosition()->x-1,this->GetEntityPosition()->GetPosition()->y,this->GetSpecies());
                        UpdatePosition(this->GetEntityPosition()->GetPosition()->x-1,this->GetEntityPosition()->GetPosition()->y);
                    }
                    break;
                case 2: //move DOWN
                    if(isLegalMovement(movement,grid)){
                        grid->UpdateGrid(this->GetEntityPosition()->GetPosition()->x,this->GetEntityPosition()->GetPosition()->y,'.');
                        grid->UpdateGrid(this->GetEntityPosition()->GetPosition()->x+1,this->GetEntityPosition()->GetPosition()->y,this->GetSpecies());
                        UpdatePosition(this->GetEntityPosition()->GetPosition()->x+1,this->GetEntityPosition()->GetPosition()->y);
                    }                
                    break;
                case 3: //move RIGHT
                    if(isLegalMovement(movement,grid)){
                        grid->UpdateGrid(this->GetEntityPosition()->GetPosition()->x,this->GetEntityPosition()->GetPosition()->y,'.');
                        grid->UpdateGrid(this->GetEntityPosition()->GetPosition()->x,this->GetEntityPosition()->GetPosition()->y+1,this->GetSpecies());
                        UpdatePosition(this->GetEntityPosition()->GetPosition()->x,this->GetEntityPosition()->GetPosition()->y+1);  
                    }              
                    break;
                case 4: //move LEFT
                    if(isLegalMovement(movement,grid)){
                        grid->UpdateGrid(this->GetEntityPosition()->GetPosition()->x,this->GetEntityPosition()->GetPosition()->y,'.');
                        grid->UpdateGrid(this->GetEntityPosition()->GetPosition()->x,this->GetEntityPosition()->GetPosition()->y-1,this->GetSpecies());  
                        UpdatePosition(this->GetEntityPosition()->GetPosition()->x,this->GetEntityPosition()->GetPosition()->y-1);
                    }              
                    break;
            }
        }


        //checks the entity's sourrounding positions 
        void CheckSourroundings(Grid *gptr, char *AdjType, int AdjPos[][2]){
            for(int i=0; i<4; i++){
                AdjType[i] = gptr->AccessGridPosition(AdjPos[i][0], AdjPos[i][1]);
            }
        }

};

//this is the werewolf class that inherits the public part of the Creature class 
class werewolf: public Creature{
    public:
        werewolf(){ this->SetSpecies(WEREWOLF); } //constructor 
        ~werewolf(){ } //destructor 

        void Heal(werewolf *w){
            int health = w->GetHealthState();
            if(health < MAX_HEALTH){
                if(rand()%2 == 0){
                    this->DecreasePotions();
                    w->SetHealth(health+1);
                }
            }
        }

        void Attack(vampire *v){
            int vdefense = v->GetDefense();
            int wattack = this->GetPower();
            if(wattack > vdefense){
                int damage = abs(wattack - vdefense);
                v->SetHealth(v->GetHealthState() - damage);
            }
        }

        void Dodge(char *AdjType, int AdjPos[][2]){
          for(int i=0; i<4; ++i){
            if(AdjType[i] == '.'){
                this->UpdatePosition(AdjPos[i][0],AdjPos[i][1]);
                break;
            }
          }
        }

};

//this is the vampire class that inherits the public part of the Creature class
//also has the diagonal movement
class vampire: public Creature{
    public:
        vampire(){ this->SetSpecies(VAMPIRE);} //contructor
        ~vampire(){ }; //destructor 

        //like the isLegalMovement function this function checks if a diagonal movement is legal
        bool isLegalDiagonalMovement(int mov,Grid *grid){
            bool flag = true;
            switch(mov){
                case 5: //UP-RIGHT
                    if(this->isInBoundary(grid) && (this->GetEntityPosition()->GetPosition()->x == 0 || this->GetEntityPosition()->GetPosition()->y == grid->getY()-1)){
                        flag = false;
                        break;
                    }
                    if(!grid->CheckPlace(this->GetEntityPosition()->GetPosition()->x-1,this->GetEntityPosition()->GetPosition()->y+1)) flag = false;                 
                    break;
                case 6: //UP-LEFT
                    if(this->isInBoundary(grid) && (this->GetEntityPosition()->GetPosition()->x == 0 || this->GetEntityPosition()->GetPosition()->y == 0)){
                        flag = false;
                        break;
                    }
                    if(!grid->CheckPlace(this->GetEntityPosition()->GetPosition()->x-1,this->GetEntityPosition()->GetPosition()->y-1)) flag = false;                
                    break;
                case 7: //DOWN-RIGHT
                    if(this->isInBoundary(grid) && (this->GetEntityPosition()->GetPosition()->x == grid->getX()-1 || this->GetEntityPosition()->GetPosition()->y == grid->getY()-1)){
                        flag = false;
                        break;
                    }
                    if(!grid->CheckPlace(this->GetEntityPosition()->GetPosition()->x+1,this->GetEntityPosition()->GetPosition()->y+1)) flag = false;                
                    break;
                case 8: //DOWN-LEFT
                    if(this->isInBoundary(grid) && (this->GetEntityPosition()->GetPosition()->x == grid->getX()-1 || this->GetEntityPosition()->GetPosition()->y == 0)){
                        flag = false;
                        break;
                    }
                    if(!grid->CheckPlace(this->GetEntityPosition()->GetPosition()->x+1,this->GetEntityPosition()->GetPosition()->y-1)) flag = false;                
                    break;
            }
            return flag;            
        }


        int Pick_Random_Vamp_Movement(){
        //this function picks a random vampire movement 
        //from 1-4 : 1 = up,2 = down,3 = rigth,4 = left 
        //from 5-8 : 5 = up-right,6 = up-left,7 = down-right,8 = down-left
            return (rand() % 8)+1;
        }
        
        //this function like the Movement() function handles the vampire's diagonal movement 
        void DiagonalMovement(int movement,Grid *grid){
            switch(movement){
                case 5: //move UP-RIGHT
                    if(isLegalDiagonalMovement(movement,grid)){
                        grid->UpdateGrid(this->GetEntityPosition()->GetPosition()->x,this->GetEntityPosition()->GetPosition()->y,'.');
                        grid->UpdateGrid(this->GetEntityPosition()->GetPosition()->x-1,this->GetEntityPosition()->GetPosition()->y+1,this->GetSpecies());
                        UpdatePosition(this->GetEntityPosition()->GetPosition()->x-1,this->GetEntityPosition()->GetPosition()->y+1);           
                    }        
                    break;
                case 6: //move UP-LEFT
                    if(isLegalDiagonalMovement(movement,grid)){
                        grid->UpdateGrid(this->GetEntityPosition()->GetPosition()->x,this->GetEntityPosition()->GetPosition()->y,'.');
                        grid->UpdateGrid(this->GetEntityPosition()->GetPosition()->x-1,this->GetEntityPosition()->GetPosition()->y-1,this->GetSpecies());
                        UpdatePosition(this->GetEntityPosition()->GetPosition()->x-1,this->GetEntityPosition()->GetPosition()->y-1);
                    }             
                    break;
                case 7: //move DOWN-RIGHT
                    if(isLegalDiagonalMovement(movement,grid)){
                        grid->UpdateGrid(this->GetEntityPosition()->GetPosition()->x,this->GetEntityPosition()->GetPosition()->y,'.');
                        grid->UpdateGrid(this->GetEntityPosition()->GetPosition()->x+1,this->GetEntityPosition()->GetPosition()->y+1,this->GetSpecies());
                        UpdatePosition(this->GetEntityPosition()->GetPosition()->x+1,this->GetEntityPosition()->GetPosition()->y+1);
                    }
                    break;
                case 8: //move DOWN-LEFT
                    if(isLegalDiagonalMovement(movement,grid)){
                        grid->UpdateGrid(this->GetEntityPosition()->GetPosition()->x,this->GetEntityPosition()->GetPosition()->y,'.');
                        grid->UpdateGrid(this->GetEntityPosition()->GetPosition()->x+1,this->GetEntityPosition()->GetPosition()->y-1,this->GetSpecies());
                        UpdatePosition(this->GetEntityPosition()->GetPosition()->x+1,this->GetEntityPosition()->GetPosition()->y-1);
                    }
                    break;
            }
        }

        //this function handles the vmapires movement 
        //if the randomly picked movement is 1-4 then do a normal movement 
        //else do a diagonal movement 
        void VampireMovement(Grid *gptr){
            int mov = Pick_Random_Vamp_Movement();
            if(mov <= 4) this->Movement(gptr);
            else DiagonalMovement(mov,gptr);
        } 

        void Heal(vampire *v){
            int health = v->GetHealthState();
            if(health < MAX_HEALTH){
                if(rand()%2 == 0){
                    this->DecreasePotions();
                    v->SetHealth(health+1);
                }
            }
        }

        void Attack(werewolf *w){
            int wdefense = w->GetDefense();
            int vattack = this->GetPower();
            if(vattack > wdefense){
                int damage = abs(vattack - wdefense);
                w->SetHealth(w->GetHealthState() - damage);
            }
        }

        void Dodge(char *AdjType, int AdjPos[][2]){
          for(int i=0; i<4; ++i){
            if(AdjType[i] == '.'){
                this->UpdatePosition(AdjPos[i][0],AdjPos[i][1]);
                break;
            }
          }
        }


};

class Entities;
//this is the avatar class that inherits everything from the character class
class avatar: public character{
    private:
        int Potions;
        char Team;
    public:
        avatar(){ //constructor 
            this->Potions = 1;
            cout<<"Pick a team : "<<endl<<"Press : 'W' for Werewolves / 'V' for Vampires."<<endl;
            char team;
            cin>>team;
            this->Team=team; 
            this->SetSpecies(this->Team);  
        }
        ~avatar(){} //destructor 

        //Increase/Decrease the avatar's potions by 1
        void IncreasePotions(){ this->Potions++; }
        void DecreasePotions(){ this->Potions--; }

        //this function heals all the teammates
        void Heal_Team(Entities *ent, Grid *grid);

        //this function gets the player input using getchar()
        char PlayerInput(){   
            char Inp;
            Inp = getchar();
            return Inp;
        }

        //get the potions number the player has and their team 
        int GetPotions(){ return this->Potions; }
        char GetTeam(){ return this->Team; }

        //this function checks whether a movement is legal or not
        //returns true if legal and false if not 
        bool isLegalMovement(Grid *grid,char movement){
            bool flag = true;
            switch(movement){
                case UP:
                    if(this->GetEntityPosition()->GetPosition()->x != 0){
                        if(grid->isPotion(this->GetEntityPosition()->GetPosition()->x-1,this->GetEntityPosition()->GetPosition()->y)) break;
                    }                
                    if(this->isInBoundary(grid) && this->GetEntityPosition()->GetPosition()->x == 0 ){
                        flag = false;
                        break;
                    }
                    if(!grid->CheckPlace(this->GetEntityPosition()->GetPosition()->x-1,this->GetEntityPosition()->GetPosition()->y)) flag = false;
                    break;
                case DOWN:
                    if(this->GetEntityPosition()->GetPosition()->x != grid->getX()-1){
                        if(grid->isPotion(this->GetEntityPosition()->GetPosition()->x+1,this->GetEntityPosition()->GetPosition()->y)) break;
                    }                 
                    if(this->isInBoundary(grid) && this->GetEntityPosition()->GetPosition()->x == grid->getX()-1){
                        flag = false;
                        break;
                    }
                    if(!grid->CheckPlace(this->GetEntityPosition()->GetPosition()->x+1,this->GetEntityPosition()->GetPosition()->y)) flag = false;
                    break;
                case RIGHT:
                    if(this->GetEntityPosition()->GetPosition()->y != grid->getY()-1){
                        if(grid->isPotion(this->GetEntityPosition()->GetPosition()->x,this->GetEntityPosition()->GetPosition()->y+1)) break;
                    }                 
                    if(this->isInBoundary(grid) && this->GetEntityPosition()->GetPosition()->y == grid->getY()-1){ 
                        flag = false;
                        break;
                    }
                    if(!grid->CheckPlace(this->GetEntityPosition()->GetPosition()->x,this->GetEntityPosition()->GetPosition()->y+1)) flag = false; 
                    break;
                case LEFT:
                    if(this->GetEntityPosition()->GetPosition()->y != 0){
                        if(grid->isPotion(this->GetEntityPosition()->GetPosition()->x,this->GetEntityPosition()->GetPosition()->y-1)) break;
                    }                 
                    if(this->isInBoundary(grid) && this->GetEntityPosition()->GetPosition()->y == 0) {
                        flag = false;
                        break;
                    }
                    if(!grid->CheckPlace(this->GetEntityPosition()->GetPosition()->x,this->GetEntityPosition()->GetPosition()->y-1)) flag = false;
                    break;
            }
            return flag;
        }


    
        void PlayerMovement(char input,Grid *gptr){
            switch(input){
                case UP:
                    if(isLegalMovement(gptr,UP)){
                        if(gptr->isPotion(this->GetEntityPosition()->GetPosition()->x-1,this->GetEntityPosition()->GetPosition()->y))this->Potions++;
                        gptr->UpdateGrid((this->GetEntityPosition()->GetPosition()->x),(this->GetEntityPosition()->GetPosition()->y),'.'); 
                        gptr->UpdateGrid((this->GetEntityPosition()->GetPosition()->x)-1,this->GetEntityPosition()->GetPosition()->y,this->GetSpecies());
                        UpdatePosition(this->GetEntityPosition()->GetPosition()->x-1,this->GetEntityPosition()->GetPosition()->y);
                        //break;
                    }                  
                    break;
                case DOWN:                        
                    if(isLegalMovement(gptr,DOWN)){
                        if(gptr->isPotion(this->GetEntityPosition()->GetPosition()->x+1,this->GetEntityPosition()->GetPosition()->y))this->Potions++;
                        gptr->UpdateGrid((this->GetEntityPosition()->GetPosition()->x),(this->GetEntityPosition()->GetPosition()->y),'.');
                        gptr->UpdateGrid((this->GetEntityPosition()->GetPosition()->x)+1,this->GetEntityPosition()->GetPosition()->y,this->GetSpecies()); 
                        UpdatePosition(this->GetEntityPosition()->GetPosition()->x+1,this->GetEntityPosition()->GetPosition()->y);
                    }
                    break;
                case RIGHT:                      
                    if(isLegalMovement(gptr,RIGHT)){
                        if(gptr->isPotion(this->GetEntityPosition()->GetPosition()->x,this->GetEntityPosition()->GetPosition()->y+1))this->Potions++;
                        gptr->UpdateGrid((this->GetEntityPosition()->GetPosition()->x),(this->GetEntityPosition()->GetPosition()->y),'.');
                        gptr->UpdateGrid(this->GetEntityPosition()->GetPosition()->x,(this->GetEntityPosition()->GetPosition()->y)+1,this->GetSpecies()); 
                        UpdatePosition(this->GetEntityPosition()->GetPosition()->x,this->GetEntityPosition()->GetPosition()->y+1);
                    }
                    break;
                case LEFT:                            
                    if(isLegalMovement(gptr,LEFT)){
                        if(gptr->isPotion(this->GetEntityPosition()->GetPosition()->x,this->GetEntityPosition()->GetPosition()->y-1))this->Potions++;
                        gptr->UpdateGrid((this->GetEntityPosition()->GetPosition()->x),(this->GetEntityPosition()->GetPosition()->y),'.');
                        gptr->UpdateGrid(this->GetEntityPosition()->GetPosition()->x,(this->GetEntityPosition()->GetPosition()->y)-1,this->GetSpecies());
                        UpdatePosition(this->GetEntityPosition()->GetPosition()->x,this->GetEntityPosition()->GetPosition()->y-1); 
                    }
                    break;
            }        
        }

};


void avatar::Heal_Team(Entities *ent,Grid *grid){
    
    char Team = this->GetTeam();
    char Time = grid->GetTime();
    int Potions = this->GetPotions();

    if(Potions > 0){
        if (Team == 'V' && Time == 'N'){
            vector<vampire *> TeammateVector;
            vector<vampire *>::iterator viter;
            TeammateVector = ent->GetVampires();
            for(viter = TeammateVector.begin(); viter != TeammateVector.end(); ++viter){
                (*viter)->SetHealth(MAX_HEALTH);
            }
        }
        else if(Team == 'W' && Time == 'D'){
            vector<werewolf *> TeammateVector;
            vector<werewolf *>::iterator witer;
            TeammateVector = ent->GetWerewolves();
            for(witer = TeammateVector.begin(); witer != TeammateVector.end(); ++witer){
                (*witer)->SetHealth(MAX_HEALTH);
            }
        }
        this->DecreasePotions();
    }
    
}


//this class will store all the vampires and the werewolves of the game 
class Entities{
    private:
        int VampCount;
        int WolfCount;
        vector<vampire *> VampVector;
        vector<werewolf *> WolfVector;

    public:
        Entities(){
            this->VampCount = 0;
            this->WolfCount = 0;
        }
        void CreateEntities(Grid *grid){
            vampire *vptr = NULL;
            werewolf *wptr = NULL;
            for(int i=0;i<grid->TotalEntitiesNum();i++){
                vptr = new vampire();
                vptr->StartingPoint(grid);
                wptr = new werewolf();
                wptr->StartingPoint(grid);
                VampVector.push_back(vptr);
                WolfVector.push_back(wptr);
            }
        }

        //also create a destructor

        void UpdateCount(){
            this->WolfCount = this->WolfVector.size();
            this->VampCount = this->VampVector.size();
        }

        int GetVampCount(){ return this->VampCount; }
        int GetWolfCount(){ return this->WolfCount; }

        vector<vampire *> GetVampires(){ return this->VampVector; }
        vector<werewolf *> GetWerewolves(){ return this->WolfVector; }

        vampire *GetVamp(int x, int y){
            vector<vampire *>::iterator viter;
            for(viter = this->VampVector.begin(); viter != this->VampVector.end(); ++viter){
                Position *vpos = (*viter)->GetEntityPosition();
                if((vpos->GetPosition()->x == x) && (vpos->GetPosition()->y == y)){
                    return *viter;
                }
            }
            return NULL;
        }
        werewolf *GetWolf(int x, int y){
            vector<werewolf *>::iterator witer;
            for(witer = this->WolfVector.begin(); witer != this->WolfVector.end(); ++witer){
                Position *wpos = (*witer)->GetEntityPosition();
                if((wpos->GetPosition()->x == x) && (wpos->GetPosition()->y == y)){
                    return *witer;
                }
            }
            return NULL;
        }

        void EntitiesMovement(Grid *grid){
            vector<vampire *>::iterator viter;
            vector<werewolf *>::iterator witer;
            for(viter = this->VampVector.begin(); viter != this->VampVector.end(); ++viter){
                //srand(time(NULL));
                (*viter)->VampireMovement(grid);
            }
            for(witer = this->WolfVector.begin(); witer != this->WolfVector.end(); ++witer){
                //srand(time(NULL));
                (*witer)->Movement(grid);
            }            
        }
        
        void EntitiesAction(Grid *grid){
            vector<vampire *>::iterator viter;
            vector<werewolf *>::iterator witer;

            //for every vampire currently in the game
            for(viter = this->VampVector.begin(); viter != this->VampVector.end(); ++viter){
                Position * pos = (*viter)->GetEntityPosition();
                int curX = pos->GetPosition()->x;
                int curY = pos->GetPosition()->y;
                int curHealth = (*viter)->GetHealthState();
                int curPotions = (*viter)->GetPotions();

                char *AdjType = new char[4]; // array that stores the type of the entities in all adjacent positions to the vampire's current position
                int AdjPos[4][2]; // array that stores the coordinates of all adjacent positions to the vampire's current position
                
                // Adjacent Position Initialization
                //Up
                AdjPos[0][0] = curX-1;
                AdjPos[0][1] = curY;
                //Down
                AdjPos[1][0] = curX+1;
                AdjPos[1][1] = curY;
                //Left
                AdjPos[2][0] = curX;
                AdjPos[2][1] = curY-1;
                //Right 
                AdjPos[3][0] = curX;
                AdjPos[3][1] = curY+1;


                (*viter)->CheckSourroundings(grid, AdjType, AdjPos);

                for(int i=0; i<4; i++){
                    if(AdjType[i] == VAMPIRE){
                        // vampire *AdjV = GetVamp(AdjPos[i][0], AdjPos[i][1]);
                        // int vhealth = AdjV->GetHealthState();
                        (*viter)->Heal(GetVamp(AdjPos[i][0], AdjPos[i][1]));
                        // (*viter)->DecreasePotions();
                    }
                    else if(AdjType[i] == WEREWOLF){
                        werewolf *wolf = GetWolf(AdjPos[i][0], AdjPos[i][1]);


                        int wp = wolf->GetPower();
                        int vp = (*viter)->GetPower();

                        if(vp >= wp) 
                            (*viter)->Attack(wolf); 
                            if((wolf->GetHealthState()) <= 0){
                                //find wolf's index in vector
                                for(int i=0; i<WolfVector.size(); ++i){
                                    if(WolfVector[i] == wolf)
                                    break;
                                }
                                //vectorpop
                                // WolfVector.erase(remove(WolfVector.begin(), WolfVector.end(), wolf), WolfVector.end());
                                WolfVector.erase(WolfVector.begin()+i);
                                //vector Size? wolf count 

                                grid->UpdateGrid(AdjPos[i][0], AdjPos[i][1],'.');

                            }
                        else
                            (*viter)->Dodge(AdjType, AdjPos);

                    }
                }
            }
            //for every werewolf
            for(witer = this->WolfVector.begin(); witer != this->WolfVector.end(); ++witer){
                // (*witer)->CheckSourroundings(grid);




            }            



        }
        
};


//this class handles the game stats that will be displayed when the game is paused
class Statistics{
    private:
        int VampiresNum;  //number of active vampires 
        int WerewolvesNum; //number of active werewolves
        int PotionsNum; //number of potions that the player has 

    public:
        Statistics(){ //constructor 
            this->VampiresNum=0;
            this->WerewolvesNum=0;
            this->PotionsNum=0;
        }
        ~Statistics(){ } //destructor 

        //this function counts the active vampires
        //the number of active vampires is the size of the vampire vector(in the Entities class)
        void CountVampires(Entities *en){
            en->UpdateCount();
            this->VampiresNum = en->GetVampCount();
        }

        //this function counts the active werewolves
        //the number of active werewolves is the size of the werewolf vector(in the Entities class)        
        void CountWerewolves(Entities *en){
            en->UpdateCount();
            this->WerewolvesNum = en->GetWolfCount();
        } 

        //this function counts the potions the player has 
        //as a parameter it receives an int number that is the potions private member of the avatar class
        void CountPotions(int potions){ this->PotionsNum = potions; }

        //get vampires,werewolves,potions number
        int getVampNum(){ return this->VampiresNum; }  
        int getWereNum(){ return this->WerewolvesNum; }
        int getPotionsNum(){ return this->PotionsNum; } 

        //function that displays the game stats when the game is paused
        void ShowStats(){
            system("stty cooked");
            cout<<"The number of active vampires is : "<<this->getVampNum()<<endl;
            cout<<"The number of active werewolves is : "<<this->getWereNum()<<endl;
            cout<<"The number of potions the player has is : "<<this->getPotionsNum()<<endl;
            return;
        }      

};



//function that creates the word/map
Grid CreateWorld(int x,int y){
    Grid grd1(x,y);
    grd1.Init(); //initialize the grid 
    grd1.PlaceTrees(); //generate random trees in the map(grid)
    grd1.PlaceWater(); //generate random rivers in the map(grid)
    grd1.PotionGenerator(); //generates a random potion in the map(grid)
    return grd1;
}


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



//this function waits for the player to press 'S' to start the gameplay 
void Game::Start(){
    cout << "To start the game press S..."<<endl;
    char in;
    cin >> in;
    while(in != 'S'){
        cout <<"Need to press 'S'!"<<endl;
        cin >> in;
    }    
}


//this function waits for the player to press 'R' to resume the gameplay
void Game::Resume(avatar *player){
    while(player->PlayerInput()!= 'R') cout << "Need to press 'R' to resume the game!"<<endl; 
}

//this function displays the game stats when the game is paused
void Game::Pause(Entities *en,Statistics *stats,avatar * player){
    char input;
    stats->CountVampires(en);
    stats->CountWerewolves(en);
    stats->CountPotions(player->GetPotions()); 
    cout << endl;
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
            cout << "***GAME-OVER***" << endl;
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
            //increase the cycle count after each frame
            gptr->IncreaseCycleCount();
            gptr->ShowGrid();
            //usleep(100000); 
            usleep(50000);
        }

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




int main(int argc,char *argv[]){

    //wrong input handling
    if(argc < 2){
        cout<<"Wrong Input!!"<<endl<<"Exepcted input is : ./game2 [integer] [integer]"<<endl;
        return -1; 
    }

    //create a game and start playing 
    Game game;
    game.CreateGame(atoi(argv[1]),atoi(argv[2]));

    return 0;
}