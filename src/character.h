#ifndef CHARACTER_H
#define CHARACTER_H

#include "element.h"
#include "location.h"
#include "weapons.h"
#include "skills.h"

#define ENEMY_COUNT 6
typedef struct Character Character;
struct Character {
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

#endif //CHARACTER_H
