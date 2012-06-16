#ifndef ENEMY_H
#define ENEMY_H

#include "element.h"
#include "skills.h"
#include "weapons.h"

#define ENEMY_COUNT 6
typedef struct Enemy Enemy;
struct Enemy {
	char *name;
	int health;
	// TODO add more stats
	Element elemental_type;	/* dominant energy type */
	int elements[ELEM_COUNT]; // use enum Element to access
	Weapons *weapon;
	Skills *skill;
};

#endif //ENEMY_H
