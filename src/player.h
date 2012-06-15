#ifndef PLAYER_H
#define PLAYER_H

#include "location.h"
#include "weapons.h"
#include "skills.h"

typedef struct Player Player;
struct Player {
	int id;
	char *name;
	Location *location;
	unsigned int action_points;
	unsigned int experience;
	int money;
	// TODO add more stats 
	int health;
	int max_health;
	int elemental_type;
	int wood;
	int fire;
	int earth;
	int metal;
	int water;
	Weapons *weapon;
	Skills *skill;
};

#endif //PLAYER_H
