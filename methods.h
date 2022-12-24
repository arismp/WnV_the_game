#ifndef methods_H
#define methods_H

class character{

    private:
        char Species; //'v' for vmapires 'w', for werewolves and 'V'/'W' for the avatar depending on the team they support

    public:
        character();
        void SetSpecies(char S);
        char GetSpecies();    
};

class werewolf: public character{
    private:
        int health;
        int power;
        int defense;
        int potions;
        int id;
        Position *pos;  
    public:
        werewolf();
        int GetHealthState();
        int GetPower();
        int GetDefense();
        int GetPotions();

        void UpdatePosition(int x, int y);
        void UpdateHealthState(int h);
        void DecreasePotions();

        Position *GetPosition();

        void StartingPoint(Grid *grid);

        bool isInBoundary(Grid *grid);
        bool isLegalMovement(char mov,Grid *grid);
        int Pick_Random_Movement();
        void WerewolfMovement(Grid *grid);

        void CheckSourroundings(Grid *gptr, char *Type, int Position[][2]);
        void Attack(vampire *);
        void Dodge(char *Type, int Position[][2]);
        void Heal(werewolf *w);

};


#endif