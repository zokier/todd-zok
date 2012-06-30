/* NOTE: maximum weapon name length is currently 14 characters:
max_weapon_nam
-------------
axe of assault
*/
#include "weapons.h"
#include "skills.h"

Weapons weapons_enemy[3] = {
	{
		0,
		"Claw",
		ELEM_EARTH,
		5,
		0
	},

	{
		1,
		"Fire breath",
		ELEM_FIRE,
		8,
		0
	},

	{
		3,
		"Bite of death",
		ELEM_EARTH,
		10,
		0
	}
};

Weapons weapons_list[WEAPON_COUNT] = {
	{
		0,
		"Stick of doom",
		ELEM_WOOD,
		3,
		9
	},

	{
		1,
		"Axe of assault",
		ELEM_METAL,
		5,
		100
	},
	{
		999,
		"Bare hands",
		ELEM_EARTH,
		1,
		0
	}
};

