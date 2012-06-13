#ifndef PLAYER_H
#define PLAYER_H

#include "location.h"

typedef struct Player Player;
struct Player {
	int id;
	char *name;
	Location *location;
	unsigned int action_points;
	unsigned int experience;
	unsigned int max_health;
	int health;
	int money;
	// TODO add more stats 
	int wood;
	int fire;
	int earth;
	int metal;
	int water;
};

#endif //PLAYER_H
