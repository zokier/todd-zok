#ifndef ENEMY_H
#define ENEMY_H

#include "skills.h"
#include "weapons.h"

#define ENEMY_NR 6
typedef struct Enemy Enemy;
struct Enemy {
	char *name;
	int health;
        // TODO add more stats
	int elemental_type;	/* dominant energy type */
        int wood;
        int fire;
        int earth;
        int metal;
        int water;
        Weapons *weapon;
 	Skills *skill;
};

#endif //ENEMY_H
