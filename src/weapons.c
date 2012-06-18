#include "weapons.h"
#include "skills.h"

Weapons weapons_enemy[3] = {
	{
		0,
		"Claw",
		ELEM_EARTH,
		2,
		0
	},

	{
		1,
		"Fire breath",
		ELEM_FIRE,
		4,
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
		"Bare hands",
		ELEM_EARTH,
		1,
		0
	},

	{
		1,
		"Stick of mayhem",
		ELEM_WOOD,
		3,
		9
	},

	{
		2,
		"Axe of assault",
		ELEM_METAL,
		5,
		100
	},

};

