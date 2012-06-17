#include "globals.h"
#include "character.h"

Character enemylist[2][ENEMY_COUNT] = {{
	{
		.name = "Wood elemental baby",
		.health = 10,
		.elements = {
			10,
			2,
			2,
			2,
			2
		},
		.weapon = &weapons_enemy[0],
		.skill[0] = &skills_list[1]
	},

	{
		.name = "Wolf cub",
		.health = 7,
		.elements = {
			4,
			2,
			2,
			2,
			2
		},
		.weapon = &weapons_enemy[0],
		.skill[0] = &skills_list[0]
	},

	{
		.name = "Wild boar",
		.health = 10,
		.elements = {
			5,
			2,
			2,
			2,
			2
		},
		.weapon = &weapons_enemy[0],
		.skill[0] = &skills_list[0]
	},

	{
		.name = "Wolf mother",
		.health = 15,
		.elements = {
			7,
			3,
			3,
			3,
			3,
		},
		.weapon = &weapons_enemy[0],
		.skill[0] = &skills_list[0]
	},

	{
		.name = "Woodland critter",
		.health = 4,
		.elements = {
			3,
			1,
			1,
			1,
			1
		},
		.weapon = &weapons_enemy[0],
		.skill[0] = &skills_list[0]
	},

	{
		.name = "Bear",
		.health = 25,
		.elements = {
			9,
			4,
			4,
			4,
			4
		},
		.weapon = &weapons_enemy[0],
		.skill[0] = &skills_list[0]
	}
},

{
	{
		.name = "Wood elemental baby",
		.health = 10,
		.elements = {
			5,
			2,
			2,
			2,
			2
		},
		.weapon = &weapons_enemy[0],
		.skill[0] = &skills_list[1]
	},

	{
		.name = "Fire elemental baby",
		.health = 10,
		.elements = {
			2,
			5,
			2,
			2,
			2
		},
		.weapon = &weapons_enemy[1],
		.skill[0] = &skills_list[0]
	},

	{
		.name = "Earth elemental baby",
		.health = 10,
		.elements = {
			2,
			2,
			5,
			2,
			2
		},
		.weapon = &weapons_list[0],
		.skill[0] = &skills_list[0]
	},

	{
		.name = "Metal elemental baby",
		.health = 10,
		.elements = {
			2,
			2,
			2,
			5,
			2
		},
		.weapon = &weapons_list[0],
		.skill[0] = &skills_list[0]
	},

	{
		.name = "Water elemental baby",
		.health = 10,
		.elements = {
			2,
			2,
			2,
			2,
			5
		},
		.weapon = &weapons_list[0],
		.skill[0] = &skills_list[0]
	},

	{
		.name = "Touho the dog",
		.health = 99,
		.elements = {
			99,
			99,
			99,
			99,
			99
		},
		.weapon = &weapons_enemy[2],
		.skill[0] = &skills_list[0]
	}
}};
