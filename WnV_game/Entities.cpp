#include <cstdlib>
#include <vector>
#include <iostream>
#include <algorithm>

#include "constants.h"
#include "Entities.h"
#include "Grid.h"
// #include "Game.h"

//this is the implementation of the entities' methods
Entities::Entities(){ //constructor 
    this->VampCount = 0; 
    this->WolfCount = 0;
}

Entities::~Entities(){  //destructor

}

//this function creates all the werewolves and the vampires and adds them into the vectors
//it also places them randomly in the grid 
void Entities::CreateEntities(Grid *grid){
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



//update the counters
void Entities::UpdateCount(){
    this->WolfCount = this->WolfVector.size();
    this->VampCount = this->VampVector.size();
}

//get the counters
int Entities::GetVampCount(){ return this->VampCount; }
int Entities::GetWolfCount(){ return this->WolfCount; }

std::vector<vampire *> Entities::GetVampires(){ return this->VampVector; }
std::vector<werewolf *> Entities::GetWerewolves(){ return this->WolfVector; }

vampire *Entities::GetVamp(int x, int y){
    
    std::vector<vampire *>::iterator viter;
    for(viter = this->VampVector.begin(); viter != this->VampVector.end(); ++viter){
        Position *vpos = (*viter)->GetEntityPosition();
        if((vpos->GetPosition()->x == x) && (vpos->GetPosition()->y == y)){
            return *viter;
        }
    }
    return NULL;
}
werewolf *Entities::GetWolf(int x, int y){
    
    std::vector<werewolf *>::iterator witer;
    for(witer = this->WolfVector.begin(); witer != this->WolfVector.end(); ++witer){
        Position *wpos = (*witer)->GetEntityPosition();
        if((wpos->GetPosition()->x == x) && (wpos->GetPosition()->y == y)){
            return *witer;
        }
    }
    return NULL;
}

//this function handles the movement of each entity (vampire/werewolf) in the game 
//it iterates through the vectors and for each object it makes a movement 
void Entities::EntitiesMovement(Grid *grid){
    std::vector<vampire *>::iterator viter;
    std::vector<werewolf *>::iterator witer;
    for(viter = this->VampVector.begin(); viter != this->VampVector.end(); ++viter){
        (*viter)->VampireMovement(grid);
    }
    for(witer = this->WolfVector.begin(); witer != this->WolfVector.end(); ++witer){
        (*witer)->Movement(grid);
    }            
}

//this function handles the action of each entity in the game
//it checks whether there are teammates to be healed or enemies that need to be dealth with near the entity and acts accordingly 
void Entities::EntitiesAction(Grid *grid){
    std::vector<vampire *>::iterator viter;
    std::vector<werewolf *>::iterator witer;

    //for every vampire currently in the game
    for(viter = this->VampVector.begin(); viter != this->VampVector.end(); ++viter){
        Position * pos = (*viter)->GetEntityPosition();
        int curX = pos->GetPosition()->x;
        int curY = pos->GetPosition()->y;
        int curHealth = (*viter)->GetHealthState();
        int curPotions = (*viter)->GetPotions();

        char *AdjType = new char[4]; // array that stores the type of the entities in all adjacent positions to the vampire's current position
        int **AdjPos = new int*[4]; // array that stores the coordinates of all adjacent positions to the vampire's current position
        for(int i=0; i<4; ++i){
            AdjPos[i] = new int[2];
        }
        // Adjacent Positions Initialization
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


       //check the vampires sourroundings
        (*viter)->CheckSourroundings(grid, AdjType, AdjPos);
        for(int i=0; i<4; i++){
            if(AdjType[i] == VAMPIRE){
                (*viter)->Heal(GetVamp(AdjPos[i][0], AdjPos[i][1]));
            }
            else if(AdjType[i] == WEREWOLF){
                werewolf *wolf = GetWolf(AdjPos[i][0], AdjPos[i][1]);
                int wp = wolf->GetPower();
                int vp = (*viter)->GetPower();
                //if the vampire is at least as strong as the werewolf it chooses to attack
                if(vp >= wp){ 
                    (*viter)->Attack(wolf); 
                    //if the werewolf has been defeated, remove it from the game
                    if((wolf->GetHealthState()) <= 0){
                        //find the werewolf's index in entities vector and erase it
                        for(int i=0; i<WolfVector.size(); ++i){
                            if(WolfVector[i] == wolf)
                            break;
                        }
                        WolfVector.erase(remove(WolfVector.begin(), WolfVector.end(), wolf), WolfVector.end());
                        grid->UpdateGrid(AdjPos[i][0], AdjPos[i][1],'.');
                    }
                }
                //if not, it tries to get away 
                else{
                    (*viter)->Dodge(AdjType, AdjPos);
                }
            }
        }
    }
    // for every werewolf
    for(witer = this->WolfVector.begin(); witer != this->WolfVector.end(); ++witer){

        Position * pos = (*witer)->GetEntityPosition();
        int curX = pos->GetPosition()->x;
        int curY = pos->GetPosition()->y;
        int curHealth = (*witer)->GetHealthState();
        int curPotions = (*witer)->GetPotions();

        char *AdjType = new char[4]; // array that stores the type of the entities in all adjacent positions to the vampire's current position
        int **AdjPos = new int*[4]; // array that stores the coordinates of all adjacent positions to the vampire's current position
        for(int i=0; i<4; ++i){
            AdjPos[i] = new int[2];
        }        
        // Adjacent Positions Initialization
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

        //check the werewolves sourroundings
        (*witer)->CheckSourroundings(grid, AdjType, AdjPos);
        for(int i=0; i<4; i++){
            if(AdjType[i] == WEREWOLF){
                (*witer)->Heal(GetWolf(AdjPos[i][0], AdjPos[i][1]));
            }
            else if(AdjType[i] == VAMPIRE){
                vampire *vamp = GetVamp(AdjPos[i][0], AdjPos[i][1]);
                int vp = vamp->GetPower();
                int wp = (*witer)->GetPower();
                //if the werewolf is at least as strong as the vampire it chooses to attack
                if(wp >= vp){ 
                    (*witer)->Attack(vamp); 
                    //if the vampire has been defeated, remove it from the game
                    if((vamp->GetHealthState()) <= 0){
                        //find the vampire's index in the entities vector and erase it
                        for(int i=0; i<VampVector.size(); ++i){
                            if(VampVector[i] == vamp){
                                break;
                            }
                        }
                        VampVector.erase(remove(VampVector.begin(), VampVector.end(), vamp), VampVector.end());
                        grid->UpdateGrid(AdjPos[i][0], AdjPos[i][1],'.');
                    }
                }
                //if not, it tries to get away
                else{
                    (*witer)->Dodge(AdjType, AdjPos);
                }
            }
        }
    }
}
        

//this is the implementation of the avatar's methods
avatar::avatar(){ //constructor 
    this->Potions = 1;
    std::cout << "Pick a team : " << std::endl << "Press : 'W' for Werewolves / 'V' for Vampires." << std::endl;
    char team;
    std::cin >> team;
    this->Team=team; 
    this->SetSpecies(this->Team);  
}
avatar::~avatar(){

} //destructor 

//Increase/Decrease the avatar's potions by 1
void avatar::IncreasePotions(){ this->Potions++; }
void avatar::DecreasePotions(){ this->Potions--; }

//this function heals all the avatar's teammates
void avatar::Heal_Team(Entities *ent,Grid *grid){

    char Team = this->GetTeam();
    char Time = grid->GetTime();
    int Potions = this->GetPotions();

    if(Potions > 0){
        if (Team == 'V'){
            if(Time == 'N'){
                std::vector<vampire *> TeammateVector;
                std::vector<vampire *>::iterator viter;
                TeammateVector = ent->GetVampires();
                for(viter = TeammateVector.begin(); viter != TeammateVector.end(); ++viter){
                    (*viter)->SetHealth(MAX_HEALTH);
                }
                this->DecreasePotions();
                std::cout << std::endl << "\nHealing all teammates!!" << std::endl;
            }
            else{ 
                std::cout << "\nVampires cannot be healed during the Day!" << std::endl;
            }
        }
        else if(Team == 'W'){
            if(Time == 'D'){
                std::vector<werewolf *> TeammateVector;
                std::vector<werewolf *>::iterator witer;
                TeammateVector = ent->GetWerewolves();
                for(witer = TeammateVector.begin(); witer != TeammateVector.end(); ++witer){
                    (*witer)->SetHealth(MAX_HEALTH);
                }
                this->DecreasePotions();
                std::cout << std::endl << "\nHealing all teammates!!" << std::endl;
            }
            else{
                std::cout << "\nWerewolves cannot be healed during the Night!" << std::endl;
            }
        }
    }
    else{
        std::cout << "\nCannot heal teammates without any Potions!" << std::endl;
    }
}


//this function gets the player input using getchar()
char avatar::PlayerInput(){   
    char Inp;
    Inp = getchar();
    std::cout << std::endl;
    return Inp;
}

//get the potions number the player has and their team 
int avatar::GetPotions(){ return this->Potions; }
char avatar::GetTeam(){ return this->Team; }

//this function checks whether a movement is legal or not
//returns true if legal and false if not 
bool avatar::isLegalMovement(Grid *grid,char movement){
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

void avatar::PlayerMovement(char input,Grid *gptr){
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


//this is the implementation of the vampire's methods
vampire::vampire(){ this->SetSpecies(VAMPIRE);} //contructor
vampire::~vampire(){ }; //destructor 

//like the isLegalMovement function this function checks if a diagonal movement is legal
bool vampire::isLegalDiagonalMovement(int mov,Grid *grid){
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


int vampire::Pick_Random_Vamp_Movement(){
//this function picks a random vampire movement 
//from 1-4 : 1 = up,2 = down,3 = rigth,4 = left 
//from 5-9 : 5 = up-right,6 = up-left,7 = down-right,8 = down-left,9 = stay still
    return (rand() % 9)+1;
}

//this function like the Movement() function handles the vampire's diagonal movement 
void vampire::DiagonalMovement(int movement,Grid *grid){
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

//this function handles the vampires movement 
//if the randomly picked movement is 1-4 then do a normal movement 
//else do a diagonal movement 
void vampire::VampireMovement(Grid *gptr){
    int mov = Pick_Random_Vamp_Movement();
    if(mov <= 4) this->Movement(gptr);
    else DiagonalMovement(mov,gptr);
} 

//this function allows vampires to heal eachother
void vampire::Heal(vampire *v){
    int health = v->GetHealthState();
    if(health < MAX_HEALTH){
        if(rand()%2 == 0){
            this->DecreasePotions();
            v->SetHealth(health+1);
        }
    }
}

//this function allows vampires to attack werewolves
void vampire::Attack(werewolf *w){
    int wdefense = w->GetDefense();
    int vattack = this->GetPower();
    if(vattack > wdefense){
        std::cout << "Vampire attacks!" << std::endl;
        int damage = abs(vattack - wdefense);
        w->SetHealth(w->GetHealthState() - damage);
    }
}

//this function allows vampires to dodge from werewolves 
void vampire::Dodge(char *AdjType, int **AdjPos){
    //the vampire checks the adjacent positions array and moves to the first available land in order to distance itself from a werewolf 
    for(int i=0; i<4; ++i){
        if(AdjType[i] == '.'){
            this->UpdatePosition(AdjPos[i][0],AdjPos[i][1]);
            break;
        }
    }
}


//this is the implementation of the werewolf's methods
werewolf::werewolf(){ this->SetSpecies(WEREWOLF); } //constructor 
werewolf::~werewolf(){ } //destructor 

//this function allowes werewolves to heal each other
void werewolf::Heal(werewolf *w){
    int health = w->GetHealthState();
    if(health < MAX_HEALTH){
        if(rand()%2 == 0){
            this->DecreasePotions();
            w->SetHealth(health+1);
        }
    }
}

//this function allows werewolves to attack vampires
void werewolf::Attack(vampire *v){
    int vdefense = v->GetDefense();
    int wattack = this->GetPower();
    if(wattack > vdefense){
        std::cout << "Werewolf attacks!" << std::endl;
        int damage = abs(wattack - vdefense);
        v->SetHealth(v->GetHealthState() - damage);
    }
}

//this function allows werewolves to dodge vampires
void werewolf::Dodge(char *AdjType, int **AdjPos){
    //the werewolf checks the adjacent positions array and moves to the first available land in order to distance itself from a vampire 
    for(int i=0; i<4; ++i){
        if(AdjType[i] == '.'){
            this->UpdatePosition(AdjPos[i][0],AdjPos[i][1]);
            break;
        }
    }
}


//this is the implementation of the Creature's methods
Creature::Creature(){ //constructor
    this->health = MAX_HEALTH; //every entity starts with a full health bar 
    srand(time(NULL));
    this->power = (rand()%MAX_POWER)+1; //every entity is given a random power from 1-MAX_POWER
    this->defense = (rand()%MAX_DEFENSE)+1; //every entity is given a random defense from 1-MAX_DEFENSE
}   
Creature::~Creature(){

}

//set the new entity's health status 
void Creature::SetHealth(int h){ this->health =h; }

//get the entity's health,power,defense and potions status
int Creature::GetHealthState(){ return this->health; }
int Creature::GetPower(){ return this->power; }
int Creature::GetDefense(){ return this->defense; }             
int Creature::GetPotions(){ return this->potions; }

//decrease the entity's potions by 1
void Creature::DecreasePotions(){ this->potions--; }

//this function check if a movement is legal (returns true if legal and false if not)
//it checks whether an entity is in the grid's boundary so it does't go out of bounds
//it also checks if the next position is land (which means it can move) 
bool Creature::isLegalMovement(char mov,Grid *grid){
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



int Creature::Pick_Random_Movement(){ 
//this function picks a random vampire movement 
//from 1-5 : 1 = up,2 = down,3 = rigth,4 = left,5 = stay still
    return (rand() % 5)+1;
}        

//this function handles the entity's movement 
//if the random movement the entity wants to make is legal then it updates the previous grid position to be land
//and the next position to be the entity's symbol
//after each movement the entity's position must also be updated
void Creature::Movement(Grid *grid){
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


//this function checks the entity's sourrounding positions and stores them into the adjacent positions array to be used from the rest of the creature's methods
void Creature::CheckSourroundings(Grid *gptr, char *AdjType, int **AdjPos){
    int x, y;
    //check all 4 adjacent positions around a creature
    for(int i=0; i<4; i++){
        x = AdjPos[i][0];
        y = AdjPos[i][1];
        //make sure the position is inside the grids boundaries
        if((x >= 0 && x <= gptr->getX()-1) && (y >= 0 && y <= gptr->getY()-1)){
            AdjType[i] = gptr->AccessGridPosition(AdjPos[i][0], AdjPos[i][1]);
        }
        else{
            AdjType[i] = OUT_OF_BOUNDS;
        }
    }
}



//this is the implementation of the cahracter classes' methods
character::character(){ //constructor 
    this->pos = new Position ();
    this->Species = '\0';   //it is initialized to '\0'
}
character::~character(){  //destructor
    delete this->pos;
}

void character::SetSpecies(char S){ this->Species = S; }
char character::GetSpecies(){ return this->Species; } 

//this function updates the entity's position after each movement
void character::UpdatePosition(int x,int y){ this->pos->SetNewPosition(x,y); } 

//this function places the entity in a random position on the grid (loop while it finds a position that is free(land))
void character::StartingPoint(Grid *grid){ 
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
Position *character::GetEntityPosition(){ return this->pos; }  

//this function checks if an entity is in the grid's boundary by checking its position's coordinates
bool character::isInBoundary(Grid *grid){
    //int x = this->GetEntityPosition()->GetPosition()->x;
    //int y = this->GetEntityPosition()->GetPosition()->y;
    int x = this->pos->GetPosition()->x;
    int y = this->pos->GetPosition()->y;
    if(x == 0 || y == 0 || x == grid->getX()-1 || y == grid->getY()-1) return true;
    return false;
}                  
