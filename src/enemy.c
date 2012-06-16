#include "enemy.h"
#include "globals.h"

Enemy enemylist[ENEMY_COUNT] = {

	{
		"Wood elemental baby",
		10,
		ELEM_WOOD,
		{
			5,
			2,
			2,
			2,
			2
		},
		&weapons_enemy[0],
		&skills_list[1]
	},

	{
		"Fire elemental baby",
		10,
		ELEM_FIRE,
		{
			2,
			5,
			2,
			2,
			2
		},
		&weapons_enemy[1],
		&skills_list[0]
	},

	{
		"Earth elemental baby",
		10,
		ELEM_EARTH,
		{
			2,
			2,
			5,
			2,
			2
		},
		&weapons_list[0],
		&skills_list[0]
	},

	{
		"Metal elemental baby",
		10,
		ELEM_METAL,
		{
			2,
			2,
			2,
			5,
			2
		},
		&weapons_list[0],
		&skills_list[0]
	},

	{
		"Water elemental baby",
		10,
		ELEM_WATER,
		{
			2,
			2,
			2,
			2,
			5
		},
		&weapons_list[0],
		&skills_list[0]
	},

	{
		"Touho the dog",
		99,
		ELEM_EARTH,
		{
			99,
			99,
			99,
			99,
			99
		},
		&weapons_enemy[2],
		&skills_list[0]}
};
