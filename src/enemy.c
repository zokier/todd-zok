#include "enemy.h"
#include "globals.h"

Enemy enemylist_0[ENEMY_COUNT] = {
	{
		"Wood elemental baby",
		10,
		ELEM_WOOD,
		{
			10,
			2,
			2,
			2,
			2
		},
		&weapons_enemy[0],
		&skills_list[1]
	},

	{
		"Wolf cub",
		7,
		ELEM_WOOD,
		{
			4,
			2,
			2,
			2,
			2
		},
		&weapons_enemy[0],
		&skills_list[0]
	},

	{
		"Wild boar",
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
		&skills_list[0]
	},

	{
		"Wolf mother",
		15,
		ELEM_WOOD,
		{
			7,
			3,
			3,
			3,
			3,
		},
		&weapons_enemy[0],
		&skills_list[0]
	},

	{
		"Woodland critter",
		4,
		ELEM_WOOD,
		{
			3,
			1,
			1,
			1,
			1
		},
		&weapons_enemy[0],
		&skills_list[0]
	},

	{
		"Bear",
		25,
		ELEM_WOOD,
		{
			9,
			4,
			4,
			4,
			4
		},
		&weapons_enemy[0],
		&skills_list[0]
	}
};


Enemy enemylist_1[ENEMY_COUNT] = {

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
