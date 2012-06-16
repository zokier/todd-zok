#ifndef PLAYER_H
#define PLAYER_H

#include "element.h"
#include "location.h"
#include "weapons.h"
#include "skills.h"

typedef struct Player Player;
struct Player {
	int id;
	char *name;
	Location *location;
	int action_points;
	int experience;
	int money;
	// TODO add more stats 
	int health;
	int max_health;
	Element elemental_type;
	int elements[ELEM_COUNT]; // use enum Element to access
	Weapons *weapon;
	Skills *skill;
	int dungeon_lvl; 
};

#endif //PLAYER_H
