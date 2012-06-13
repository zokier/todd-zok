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
	int money;
	// TODO add more stats 
	int wood;
	int fire;
	int earth;
	int metal;
	int water;
	int weapon_index; /* currently only 1 weapon and 0 skills is used. Here should be a struct Skills[4] or something */
};

#endif //PLAYER_H
