#ifndef WEAPONS_H
#define WEAPONS_H

#define WEAPON_NR 3 /* amount of weapons, 0 is the first. TODO: 2 here means 2 weapons, not 3. Why? */
typedef struct Weapons Weapons;
struct Weapons {
	int index;
	int dmg_type;
	char name[20];
};

/* damage types */
#define DMG_WOOD 1
#define DMG_FIRE 2
#define DMG_EARTH 3
#define DMG_METAL 4
#define DMG_WATER 5

#endif //PLAYER_H
 
