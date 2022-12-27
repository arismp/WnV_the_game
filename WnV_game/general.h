#ifndef general_H
#define general_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <unistd.h>

#include "Entities.h"
#include "Grid.h"
#include "Game.h"

using namespace std;

const char UP = 'w';   //Up movement
const char DOWN = 's'; //Down movement 
const char RIGHT = 'd'; //rigth movement
const char LEFT = 'a'; //left movement 

const int MAX_POWER = 3;  //maximum power is 3
const int MAX_HEALTH = 3; //maximum health is 3
const int MAX_DEFENSE = 3; //maximum defense is 3
const int MAX_POTIONS = 2; //maximum number of potions is 2 

const char WEREWOLF = 'w'; 
const char VAMPIRE = 'v';

const char WATER = '=';
const char TREE = 'T';
const char EARTH = ' ';

#endif