#ifndef WEAPONS_H
#define WEAPONS_H

#define WEAPON_NR 4 /* amount of weapons, 0 is the first. TODO: 2 here means 2 weapons, not 3. Why? */

typedef struct Weapons Weapons;
struct Weapons {
	int index;
	char name[20];
	int dmg_type;
	int damage;
};

/* damage types */
#define TYPE_WOOD 1
#define TYPE_FIRE 2
#define TYPE_EARTH 3
#define TYPE_METAL 4
#define TYPE_WATER 5

#endif //PLAYER_H
 
