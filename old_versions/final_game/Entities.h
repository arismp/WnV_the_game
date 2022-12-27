//this file contains the declarations of the following classes: 
//  character, avatar, Creature, Entities, vampire, werewolf
#ifndef Entities_H
#define Entities_H

#include <vector>
// #include "Game.h"
#include "Grid.h"


//forward declarations of the classes
class Entities;
class vampire;
//maybe a virtual function 
//also has the position of the object on the grid 
class character{

    private:
        char Species; //'v' for vmapires 'w', for werewolves and 'V'/'W' for the avatar depending on the team they support
        Position *pos;

    public:
        character();
        ~character();

        void SetSpecies(char S);
        char GetSpecies();

        void UpdatePosition(int x,int y);
        void StartingPoint(Grid *grid);
        Position *GetEntityPosition();
        bool isInBoundary(Grid *grid);
};


//this is the avatar class that inherits everything from the character class
class avatar: public character{
    private:
        int Potions;
        char Team;
    public:
        avatar();
        ~avatar();

        char GetTeam();
        int GetPotions();
        
        void IncreasePotions();
        void DecreasePotions();
        void Heal_Team(Entities *ent, Grid *grid);

        char PlayerInput();

        bool isLegalMovement(Grid *grid,char movement);
        void PlayerMovement(char input,Grid *gptr);

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
        Creature();
        ~Creature();

        void SetHealth(int h);

        int GetHealthState();
        int GetPower();
        int GetDefense();             
        int GetPotions();

        void DecreasePotions();

        bool isLegalMovement(char mov,Grid *grid);
        int Pick_Random_Movement();
        void Movement(Grid *grid);

        void CheckSourroundings(Grid *gptr, char *AdjType, int **AdjPos);

};

//this is the werewolf class that inherits the public part of the Creature class 
class werewolf: public Creature{
    public:
        werewolf();
        ~werewolf();

        void Heal(werewolf *w);
        void Attack(vampire *v);
        void Dodge(char *AdjType, int **AdjPos);
    
};

//this is the vampire class that inherits the public part of the Creature class
//also has the diagonal movement
class vampire: public Creature{
    public:
        vampire();
        ~vampire(); 

        int Pick_Random_Vamp_Movement();
        bool isLegalDiagonalMovement(int mov,Grid *grid);
        void DiagonalMovement(int movement,Grid *grid);
        void VampireMovement(Grid *gptr);

        void Heal(vampire *v);
        void Attack(werewolf *w);
        void Dodge(char *AdjType, int **AdjPos);

};

//this class will store all the vampires and the werewolves of the game 
//it also handles the movement and the actions(attack etc.) of each entity (werewolves/vampires)
class Entities{
    private:
        int VampCount; //the number of active vampires
        int WolfCount; //the number of active werewolves
        std::vector<vampire *> VampVector; //this is a vector that stores a pointer to every vmapire object
        std::vector<werewolf *> WolfVector; //this is a vector that stores a pointer to every werewolf object

    public:
        Entities(); 
        ~Entities(); 

         
        void CreateEntities(Grid *grid);

        void UpdateCount(); 
        int GetVampCount();
        int GetWolfCount();

        std::vector<vampire *> GetVampires();
        std::vector<werewolf *> GetWerewolves();

        vampire *GetVamp(int x, int y);
        werewolf *GetWolf(int x, int y);

        void EntitiesMovement(Grid *grid);
        void EntitiesAction(Grid *grid);

};

#endif
