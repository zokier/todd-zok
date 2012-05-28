#ifndef PLAYER_H
#define PLAYER_H

#include "location.h"

typedef struct Player Player;
struct Player {
	char *name;
	Location *location;
	unsigned int action_points;
	unsigned int experience;
	unsigned int max_health;
	int health;
	int money;
	// TODO add more stats 
};

#endif //PLAYER_H
