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

        void ShowDayAndNightCycle(){ // show when is day and when is night 
            cout << endl;
            cout << " ";
            if(this->Day_n_Night_Cycle == 'D') cout << "DAY";
            else cout << "NIGHT";
            cout << endl;
        }

        char GetTime(){ return this->Day_n_Night_Cycle; }
        
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
                this->grid[randX][randY]='T';
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
                this->grid[randX][randY]='=';    
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




//add something else in this class that will be used in the sub classes
//maybe a virtual function 
//also have the position of the object to be a member of the class
class character{

    private:
        char Species; //'v' for vmapires 'w', for werewolves and 'V'/'W' for the avatar depending on the team they support

    public:
        character(){
            this->Species = '\0';   //it is initialized to '\0'
        }
        void SetSpecies(char S){ this->Species = S; }
        char GetSpecies(){ return this->Species; }      
};

//the vmapire class 
class vampire: public character{
    private:
        int health; 
        int power;
        int defense;
        int potions;
        int id;
        Position *pos;

    public:

        vampire(){   //constructor
            this->pos = new Position();
            this->id = -1;
            this->health = MAX_HEALTH;
            srand(time(NULL));
            this->power = rand()%MAX_POWER;
            this->defense = rand()%MAX_DEFENSE;
            this->potions = rand()%MAX_POTIONS;
            this->SetSpecies(VAMPIRE);
        }

        ~vampire(){ }; //destructor 

        void UpdatePosition(int x,int y){ this->pos->SetNewPosition(x,y); }
        
        //this function places the vampire in a random position in the grid(loop while a random position is empty)
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

        //this function returns the vampire's position 
        Position *GetPosition(){ return this->pos; }

        //get vampire's health,power,defense
        int GetHealthState(){ return this->health; }
        int GetPower(){ return this->power; }
        int GetDefense(){ return this->defense; }
        int GetPotions(){ return this->potions; }

        bool isInBoundary(Grid *grid){
            int x = pos->GetPosition()->x;
            int y = pos->GetPosition()->y;
            if(x == 0 || y == 0 || x == grid->getX()-1 || y == grid->getY()-1) return true;
            return false;
        }

        //this function checks if a movement is legal for the vampire to make 
        //the vapmire isn't able to go through trees or water puddles 
        //is not able to go through other entities or the player
        //is not able to go outside the grid 
        //if a movement is illegal then the function returns false and true if legal 
        bool isLegalMovement(char mov,Grid *grid){
            bool flag = true;
            switch(mov){
                case 1: //UP
                    if(isInBoundary(grid) && pos->GetPosition()->x == 0 ){
                        flag = false;
                        break;
                    }
                    if(!grid->CheckPlace(pos->GetPosition()->x-1,pos->GetPosition()->y)) flag = false;                    
                    break;
                case 2: //DOWN
                    if(isInBoundary(grid) && pos->GetPosition()->x == grid->getX()-1){
                        flag = false;
                        break;
                    }
                    if(!grid->CheckPlace(pos->GetPosition()->x+1,pos->GetPosition()->y)) flag = false;                
                    break;
                case 3: //RIGHT
                     if(isInBoundary(grid) && pos->GetPosition()->y == grid->getY()-1){
                        flag = false;
                        break;
                    }
                    if(!grid->CheckPlace(pos->GetPosition()->x,pos->GetPosition()->y+1)) flag = false;               
                    break;
                case 4: //LEFT
                    if(isInBoundary(grid) && pos->GetPosition()->y == 0){
                        flag = false;
                        break;
                    }
                    if(!grid->CheckPlace(pos->GetPosition()->x,pos->GetPosition()->y-1)) flag = false;                
                    break;
                case 5: //UP-RIGHT
                    if(isInBoundary(grid) && (pos->GetPosition()->x == 0 || pos->GetPosition()->y == grid->getY()-1)){
                        flag = false;
                        break;
                    }
                    if(!grid->CheckPlace(pos->GetPosition()->x-1,pos->GetPosition()->y+1)) flag = false;                 
                    break;
                case 6: //UP-LEFT
                    if(isInBoundary(grid) && (pos->GetPosition()->x == 0 || pos->GetPosition()->y == 0)){
                        flag = false;
                        break;
                    }
                    if(!grid->CheckPlace(pos->GetPosition()->x-1,pos->GetPosition()->y-1)) flag = false;                
                    break;
                case 7: //DOWN-RIGHT
                    if(isInBoundary(grid) && (pos->GetPosition()->x == grid->getX()-1 || pos->GetPosition()->y == grid->getY()-1)){
                        flag = false;
                        break;
                    }
                    if(!grid->CheckPlace(pos->GetPosition()->x+1,pos->GetPosition()->y+1)) flag = false;                
                    break;
                case 8: //DOWN-LEFT
                    if(isInBoundary(grid) && (pos->GetPosition()->x == grid->getX()-1 || pos->GetPosition()->y == 0)){
                        flag = false;
                        break;
                    }
                    if(!grid->CheckPlace(pos->GetPosition()->x+1,pos->GetPosition()->y-1)) flag = false;                
                    break;
            }
            return flag;
        }

        int Pick_Random_Movement(){
        //this function picks a random movement 
            //srand(time(NULL));   
            return rand() % 8;
        }
        
        //vampire's normal movement ( up , down , right , left)
        void NormalMovement(int movement,Grid *grid){
            switch(movement){
                case 1: //move UP
                    if(isLegalMovement(movement,grid)){
                        grid->UpdateGrid(pos->GetPosition()->x,pos->GetPosition()->y,'.');
                        grid->UpdateGrid(pos->GetPosition()->x-1,pos->GetPosition()->y,this->GetSpecies());
                        UpdatePosition(pos->GetPosition()->x-1,pos->GetPosition()->y);
                    }
                    break;
                case 2: //move DOWN
                    if(isLegalMovement(movement,grid)){
                        grid->UpdateGrid(pos->GetPosition()->x,pos->GetPosition()->y,'.');
                        grid->UpdateGrid(pos->GetPosition()->x+1,pos->GetPosition()->y,this->GetSpecies());
                        UpdatePosition(pos->GetPosition()->x+1,pos->GetPosition()->y);
                    }                
                    break;
                case 3: //move RIGHT
                    if(isLegalMovement(movement,grid)){
                        grid->UpdateGrid(pos->GetPosition()->x,pos->GetPosition()->y,'.');
                        grid->UpdateGrid(pos->GetPosition()->x,pos->GetPosition()->y+1,this->GetSpecies());
                        UpdatePosition(pos->GetPosition()->x,pos->GetPosition()->y+1);  
                    }              
                    break;
                case 4: //move LEFT
                    if(isLegalMovement(movement,grid)){
                        grid->UpdateGrid(pos->GetPosition()->x,pos->GetPosition()->y,'.');
                        grid->UpdateGrid(pos->GetPosition()->x,pos->GetPosition()->y-1,this->GetSpecies());  
                        UpdatePosition(pos->GetPosition()->x,pos->GetPosition()->y-1);
                    }              
                    break;
            }
        }

        //vampire's diagonal movement (up-rigth , up-left , down-right , down-left)
        void DiagonalMovement(int movement,Grid *grid){
            switch(movement){
                case 5: //move UP-RIGHT
                    if(isLegalMovement(movement,grid)){
                        grid->UpdateGrid(pos->GetPosition()->x,pos->GetPosition()->y,'.');
                        grid->UpdateGrid(pos->GetPosition()->x-1,pos->GetPosition()->y+1,this->GetSpecies());
                        UpdatePosition(pos->GetPosition()->x-1,pos->GetPosition()->y+1);           
                    }        
                    break;
                case 6: //move UP-LEFT
                    if(isLegalMovement(movement,grid)){
                        grid->UpdateGrid(pos->GetPosition()->x,pos->GetPosition()->y,'.');
                        grid->UpdateGrid(pos->GetPosition()->x-1,pos->GetPosition()->y-1,this->GetSpecies());
                        UpdatePosition(pos->GetPosition()->x-1,pos->GetPosition()->y-1);
                    }             
                    break;
                case 7: //move DOWN-RIGHT
                    if(isLegalMovement(movement,grid)){
                        grid->UpdateGrid(pos->GetPosition()->x,pos->GetPosition()->y,'.');
                        grid->UpdateGrid(pos->GetPosition()->x+1,pos->GetPosition()->y+1,this->GetSpecies());
                        UpdatePosition(pos->GetPosition()->x+1,pos->GetPosition()->y+1);
                    }
                    break;
                case 8: //move DOWN-LEFT
                    if(isLegalMovement(movement,grid)){
                        grid->UpdateGrid(pos->GetPosition()->x,pos->GetPosition()->y,'.');
                        grid->UpdateGrid(pos->GetPosition()->x+1,pos->GetPosition()->y-1,this->GetSpecies());
                        UpdatePosition(pos->GetPosition()->x+1,pos->GetPosition()->y-1);
                    }
                    break;
            }
        }

        //function that controls the vampire's movement
        //if the randomly picked movement is 1-4 then the vampire does a normal movement 
        //if the randomly picked movement is 5-8 then the vampire does a diagonal movement 
        void VampireMovement(Grid *gptr){
            int mov = Pick_Random_Movement();
            if(mov <= 4) NormalMovement(mov,gptr);
            else DiagonalMovement(mov,gptr);
        }

        void CheckSourroundings(Grid *gptr){
            
            int curX = pos->GetPosition()->x;
            int curY = pos->GetPosition()->y;
            char AdjType[4];     // array that stores the type of the entities in all adjacent positions to the vampire's current position
            int AdjPos[4][2];   // array that stores the coordinates of all adjacent positions to the vampire's current position

            // Adjacent Position Initialization
            AdjPos[0][1] = curX-1;
            AdjPos[0][2] = curY;


            //////CHECK IF OUT OF BOUNDS 
            //check UP
            AdjType[0] = gptr->AccessGridPosition(AdjPos[0][0], AdjPos[0][1]);
            if(AdjType[0] == WEREWOLF){
                Heal(AdjPos[0][0], AdjPos[0][1]);
            }
            else if(AdjType[0] == VAMPIRE){

            }

            //check DOWN
            AdjType[1] = gptr->AccessGridPosition(curX+1, curY);

            //check LEFT
            AdjType[2] = gptr->AccessGridPosition(curX, curY-1);

            //check RIGHT
            AdjType[3] = gptr->AccessGridPosition(curX, curY+1);
            
 
        }

        void Heal(int x, int y){
            vector<vampire *> TeammateVector = ent->GetVampires();

        }

        void Attack(int x, int y){
        //*********YOUT CODE HERE**********
            return;          
        }

        void Dodge(){
        //*********YOUT CODE HERE**********
            return;        
        }

        

};

//the werewolf class
class werewolf: public character{
    private:
        int health;
        int power;
        int defense;
        int potions;
        int id;
        Position *pos;        
    public:
        werewolf(){ //constructor 
            this->pos = new Position();
            this->id = -1;
            this->health = MAX_HEALTH;
            srand(time(NULL));
            this->power = rand()%MAX_POWER;
            this->defense = rand()%MAX_DEFENSE;
            this->potions = rand()%MAX_POTIONS;
            this->SetSpecies(WEREWOLF);  
        }   

        ~werewolf(){ } //destructor 

        //get werewolf's health,power,defense
        int GetHealthState(){ return this->health; }
        int GetPower(){ return this->power; }
        int GetDefense(){ return this->defense; }     
        int GetPotions(){ return this->potions; } 

        //update the werewolf's position after each movement 
        void UpdatePosition(int x,int y){ this->pos->SetNewPosition(x,y); }
        void UpdateHealthState(int h){ this->health = h; }

        //get werewolf's position in the grid 
        Position *GetPosition(){ return this->pos; }

        //this function places the werewolf in a random position in the grid(loop while a random position is empty)
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

        bool isInBoundary(Grid *grid){
            int x = pos->GetPosition()->x;
            int y = pos->GetPosition()->y;
            if(x == 0 || y == 0 || x == grid->getX()-1 || y == grid->getY()-1) return true;
            return false;
        }       


        bool isLegalMovement(char mov,Grid *grid){
            bool flag = true;
            switch(mov){
                case 1: //UP
                    if(isInBoundary(grid) && pos->GetPosition()->x == 0 ){
                        flag = false;
                        break;
                    }
                    if(!grid->CheckPlace(pos->GetPosition()->x-1,pos->GetPosition()->y)) flag = false;                    
                    break;
                case 2: //DOWN
                    if(isInBoundary(grid) && pos->GetPosition()->x == grid->getX()-1){
                        flag = false;
                        break;
                    }
                    if(!grid->CheckPlace(pos->GetPosition()->x+1,pos->GetPosition()->y)) flag = false;                
                    break;
                case 3: //RIGHT
                     if(isInBoundary(grid) && pos->GetPosition()->y == grid->getY()-1){
                        flag = false;
                        break;
                    }
                    if(!grid->CheckPlace(pos->GetPosition()->x,pos->GetPosition()->y+1)) flag = false;               
                    break;
                case 4: //LEFT
                    if(isInBoundary(grid) && pos->GetPosition()->y == 0){
                        flag = false;
                        break;
                    }
                    if(!grid->CheckPlace(pos->GetPosition()->x,pos->GetPosition()->y-1)) flag = false;                
                    break;
            }
            return flag;
        }



        int Pick_Random_Movement(){ 
        //this function picks a random movement
            //srand(time(NULL));
            return rand() % 4;
        }        

        void WerewolfMovement(Grid *grid){
            int movement = Pick_Random_Movement();
            switch(movement){
                case 1: //move UP
                    if(isLegalMovement(movement,grid)){
                        grid->UpdateGrid(pos->GetPosition()->x,pos->GetPosition()->y,'.');
                        grid->UpdateGrid(pos->GetPosition()->x-1,pos->GetPosition()->y,this->GetSpecies());
                        UpdatePosition(pos->GetPosition()->x-1,pos->GetPosition()->y);
                    }
                    break;
                case 2: //move DOWN
                    if(isLegalMovement(movement,grid)){
                        grid->UpdateGrid(pos->GetPosition()->x,pos->GetPosition()->y,'.');
                        grid->UpdateGrid(pos->GetPosition()->x+1,pos->GetPosition()->y,this->GetSpecies());
                        UpdatePosition(pos->GetPosition()->x+1,pos->GetPosition()->y);
                    }                
                    break;
                case 3: //move RIGHT
                    if(isLegalMovement(movement,grid)){
                        grid->UpdateGrid(pos->GetPosition()->x,pos->GetPosition()->y,'.');
                        grid->UpdateGrid(pos->GetPosition()->x,pos->GetPosition()->y+1,this->GetSpecies());
                        UpdatePosition(pos->GetPosition()->x,pos->GetPosition()->y+1);  
                    }              
                    break;
                case 4: //move LEFT
                    if(isLegalMovement(movement,grid)){
                        grid->UpdateGrid(pos->GetPosition()->x,pos->GetPosition()->y,'.');
                        grid->UpdateGrid(pos->GetPosition()->x,pos->GetPosition()->y-1,this->GetSpecies());  
                        UpdatePosition(pos->GetPosition()->x,pos->GetPosition()->y-1);
                    }              
                    break;
            }
        }

        void Attack(){
        //*********YOUR CODE HERE**********
            return;          
        }

        void Dodge(){
        //*********YOUR CODE HERE**********
            return;        
        }

        void Heal(){

        }


};

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

        void EntitiesMovement(Grid *grid){
            vector<vampire *>::iterator viter;
            vector<werewolf *>::iterator witer;
            for(viter = this->VampVector.begin(); viter != this->VampVector.end(); ++viter){
                (*viter)->VampireMovement(grid);
            }
            for(witer = this->WolfVector.begin(); witer != this->WolfVector.end(); ++witer){
                (*witer)->WerewolfMovement(grid);
            }            
        }
        
};


class avatar: public character{
    private:
        int Potions;
        char Team;
        Position *pos;
    public:
        avatar(){
            this->pos = new Position();
            this->Potions = 1;
            cout<<"Pick a team : "<<endl<<"Press : 'W' for Werewolves / 'V' for Vampires."<<endl;
            char team;
            cin>>team;
            this->Team=team; 
            this->SetSpecies(this->Team); //the icon of the player will be : this->Team 
        }
        ~avatar(){} 

        Position *GetPosition(){ return this->pos; }

        void UpdatePosition(int x,int y){ this->pos->SetNewPosition(x,y);}
        void IncreasePotions(){ this->Potions++; }
        void DecreasePotions(){ this->Potions--; }

        void StartingPoint(Grid *grid){    //maybe put this in the character class as a virtual function and add the players position as a memeber of the class and all teh position related functions at the character class
            srand(time(NULL));
            int randX,randY;
            do{
                randX=rand()%grid->getX();
                randY=rand()%grid->getY();
            }while(!(grid->CheckPlace(randX,randY)));
            grid->UpdateGrid(randX,randY,this->GetSpecies());
            UpdatePosition(randX,randY);
        }

        void Heal_Team(Entities *ent, Grid *grid);

        char PlayerInput(){   
            char Inp;
            //cin >> Inp;
            Inp = getchar();
            return Inp;
        }


        int GetPotions(){ return this->Potions; }
        char GetTeam(){ return this->Team; }

        bool isInBoundary(int x,int y,Grid *gptr){
            if(x == 0 || y == 0 || x == gptr->getX()-1 || y == gptr->getY()-1) return true; //have this at the character class
            return false;
        }

        bool isLegalMovement(Grid *grid,char movement){
            bool flag = true;
            switch(movement){
                case UP:
                    if(this->pos->GetPosition()->x != 0){
                        if(grid->isPotion(this->pos->GetPosition()->x-1,this->pos->GetPosition()->y)) break;
                    }                
                    if(isInBoundary(this->pos->GetPosition()->x,this->pos->GetPosition()->y,grid) && this->pos->GetPosition()->x == 0 ){
                        flag = false;
                        break;
                    }
                    if(!grid->CheckPlace(this->pos->GetPosition()->x-1,this->pos->GetPosition()->y)) flag = false;
                    break;
                case DOWN:
                    if(this->pos->GetPosition()->x != grid->getX()-1){
                        if(grid->isPotion(this->pos->GetPosition()->x+1,this->pos->GetPosition()->y)) break;
                    }                 
                    if(isInBoundary(this->pos->GetPosition()->x,this->pos->GetPosition()->y,grid) && this->pos->GetPosition()->x == grid->getX()-1){
                        flag = false;
                        break;
                    }
                    if(!grid->CheckPlace(this->pos->GetPosition()->x+1,this->pos->GetPosition()->y)) flag = false;
                    break;
                case RIGHT:
                    if(this->pos->GetPosition()->y != grid->getY()-1){
                        if(grid->isPotion(this->pos->GetPosition()->x,this->pos->GetPosition()->y+1)) break;
                    }                 
                    if(isInBoundary(this->pos->GetPosition()->x,this->pos->GetPosition()->y,grid) && this->pos->GetPosition()->y == grid->getY()-1){ 
                        flag = false;
                        break;
                    }
                    if(!grid->CheckPlace(this->pos->GetPosition()->x,this->pos->GetPosition()->y+1)) flag = false; 
                    break;
                case LEFT:
                    if(this->pos->GetPosition()->y != 0){
                        if(grid->isPotion(this->pos->GetPosition()->x,this->pos->GetPosition()->y-1)) break;
                    }                 
                    if(isInBoundary(this->pos->GetPosition()->x,this->pos->GetPosition()->y,grid) && this->pos->GetPosition()->y == 0) {
                        flag = false;
                        break;
                    }
                    if(!grid->CheckPlace(this->pos->GetPosition()->x,this->pos->GetPosition()->y-1)) flag = false;
                    break;
            }
            return flag;
        }


    
        void PlayerMovement(char input,Grid *gptr){
            switch(input){
                case UP:
                    if(isLegalMovement(gptr,UP)){
                        if(gptr->isPotion(this->pos->GetPosition()->x-1,this->pos->GetPosition()->y))this->Potions++;
                        gptr->UpdateGrid((this->pos->GetPosition()->x),(this->pos->GetPosition()->y),'.'); //make these  2 lines as a function and have it at the character class
                        gptr->UpdateGrid((this->pos->GetPosition()->x)-1,this->pos->GetPosition()->y,this->GetSpecies());
                        UpdatePosition(this->pos->GetPosition()->x-1,this->pos->GetPosition()->y);
                        //break;
                    }                  
                    break;
                case DOWN:                        
                    if(isLegalMovement(gptr,DOWN)){
                        if(gptr->isPotion(this->pos->GetPosition()->x+1,this->pos->GetPosition()->y))this->Potions++;
                        gptr->UpdateGrid((this->pos->GetPosition()->x),(this->pos->GetPosition()->y),'.');
                        gptr->UpdateGrid((this->pos->GetPosition()->x)+1,this->pos->GetPosition()->y,this->GetSpecies()); 
                        UpdatePosition(this->pos->GetPosition()->x+1,this->pos->GetPosition()->y);
                    }
                    break;
                case RIGHT:                      
                    if(isLegalMovement(gptr,RIGHT)){
                        if(gptr->isPotion(this->pos->GetPosition()->x,this->pos->GetPosition()->y+1))this->Potions++;
                        gptr->UpdateGrid((this->pos->GetPosition()->x),(this->pos->GetPosition()->y),'.');
                        gptr->UpdateGrid(this->pos->GetPosition()->x,(this->pos->GetPosition()->y)+1,this->GetSpecies()); 
                        UpdatePosition(this->pos->GetPosition()->x,this->pos->GetPosition()->y+1);
                    }
                    break;
                case LEFT:                            
                    if(isLegalMovement(gptr,LEFT)){
                        if(gptr->isPotion(this->pos->GetPosition()->x,this->pos->GetPosition()->y-1))this->Potions++;
                        gptr->UpdateGrid((this->pos->GetPosition()->x),(this->pos->GetPosition()->y),'.');
                        gptr->UpdateGrid(this->pos->GetPosition()->x,(this->pos->GetPosition()->y)-1,this->GetSpecies());
                        UpdatePosition(this->pos->GetPosition()->x,this->pos->GetPosition()->y-1); 
                    }
                    break;
            }        
        }

};

void avatar::Heal_Team(Entities *ent,Grid *grid){


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

        void EntitiesMovement(Grid *grid){
            vector<vampire *>::iterator viter;
            vector<werewolf *>::iterator witer;
            for(viter = this->VampVector.begin(); viter != this->VampVector.end(); ++viter){
                (*viter)->VampireMovement(grid);
            }
            for(witer = this->WolfVector.begin(); witer != this->WolfVector.end(); ++witer){
                (*witer)->WerewolfMovement(grid);
            }            
        }
  
};

//this class displays the game's statistics after the player pauses the game
class Statistics{
    private:
        int VampiresNum;  //number of active vampires 
        int WerewolvesNum; //number of active werewolves
        int PotionsNum; //number of potions that the player has 
    public:
        Statistics(){ //construstor 
            this->VampiresNum=0;
            this->WerewolvesNum=0;
            this->PotionsNum=0;
        }
        ~Statistics(){ } //destrustor 
        
        void CountVampires(Entities *en){
            en->UpdateCount();
            this->VampiresNum = en->GetVampCount();
        }

        void CountWerewolves(Entities *en){
            en->UpdateCount();
            this->WerewolvesNum = en->GetWolfCount();
        } 

        void CountPotions(int potions){ this->PotionsNum = potions; }
        int getVampNum(){ return this->VampiresNum; }  
        int getWereNum(){ return this->WerewolvesNum; }
        int getPotionsNum(){ return this->PotionsNum; } 
        
        //display the game's statistics
        void ShowStats(){
            system("stty cooked");
            cout<<"The number of active vampires is : "<<this->getVampNum()<<endl;
            cout<<"The number of active werewolves is : "<<this->getWereNum()<<endl;
            cout<<"The number of potions the player has is : "<<this->getPotionsNum()<<endl;
            return;
        }      


};



//function that creates the word/map
//maybe move this function on the Game class
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
        void Start();
        void Pause(Entities *en,Statistics *stats,avatar * player);
        void Resume(avatar *player);
        void GamePlay(Grid *gptr,avatar *player,Entities * ent);
};

//this function allows the game to start only when the player presses 'S'
void Game::Start(){
    cout << "To start the game press S..."<<endl;
    char in;
    cin >> in;
    while(in != 'S'){
        cout <<"Need to press 'S'!"<<endl;
        cin >> in;
    }    
}

//this function allows the game to resume only when the player presses 'R'
void Game::Resume(avatar *player){
    while(player->PlayerInput()!= 'R') cout << "Need to press 'R' to resume the game!"<<endl; 
}

//when the player presses 'P' the game is paused and the game's statistics are in display
void Game::Pause(Entities *en,Statistics *stats,avatar * player){
    char input;
    stats->CountVampires(en);
    stats->CountWerewolves(en);
    stats->CountPotions(player->GetPotions()); 
    stats->ShowStats();    
}

//this function controls the actual gameplay 
//it has a while loop that loops until the player presses 'Q' 
void Game::GamePlay(Grid *gptr,avatar *player,Entities * ent){

    this->flag=true;  
    
    Statistics stats;

    this->Start(); //start the game when the player presses 'S'
    gptr->ShowGrid();
    char input;

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
            player->Heal_Team(ent, gptr);
        }
        else{
            //first the player moves 
            player->PlayerMovement(input,gptr);
            //after the player's movement the other entities move 
            ent->EntitiesMovement(gptr);
            //increase the cycle count after each frame
            gptr->IncreaseCycleCount();
            //render 
            gptr->ShowGrid();
            //usleep(100000); 
            usleep(50000);
        }

    }
}






int main(int argc,char *argv[]){

    //wrong input handling
    if(argc < 2){
        cout<<"Wrong Input!!"<<endl<<"Exepcted input is : ./new_game [integer] [integer]"<<endl;
        return -1; 
    }
    //create the world 
    Grid grid = CreateWorld(atoi(argv[1]),atoi(argv[2]));
    grid.ShowGrid();

    //create the player and place them in a random position in the grid/world
    avatar Player;
    Player.StartingPoint(&grid);
    
    //create all the vampires and the werewolves of the game 
    Entities entities;
    entities.CreateEntities(&grid);


    //start the Game
    Game g;
    g.GamePlay(&grid,&Player,&entities);
    system("stty cooked");


    return 0;
}
