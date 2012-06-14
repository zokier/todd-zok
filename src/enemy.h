#ifndef ENEMY_H
#define ENEMY_H

#define TYPE_WOOD 1
#define TYPE_FIRE 2
#define TYPE_EARTH 3
#define TYPE_METAL 4
#define TYPE_WATER 5

#include "skills.h"
#define ENEMY_NR 6
typedef struct Enemy Enemy;
struct Enemy {
	char *name;
        // TODO add more stats
	int dom_type;	/* dominant energy type */
        int wood;
        int fire;
        int earth;
        int metal;
        int water;
	Skills *skill;
};

#endif //ENEMY_H
