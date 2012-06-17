#ifndef WEAPONS_H
#define WEAPONS_H

#include "element.h"

#define WEAPON_COUNT 3 /* amount of weapons, 0 is the first. TODO: 2 here means 2 weapons, not 3. Why? */

typedef struct Weapons Weapons;
struct Weapons {
	int index;	/* is index used anywhere? Can it be removed? */
	char name[20];
	Element dmg_type;
	int damage;
	int price;	/* is the shop the only place to buy / sell weapons ?? */
};

#endif //PLAYER_H
 
