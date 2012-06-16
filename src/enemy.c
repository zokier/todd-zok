#include "globals.h"
#include "character.h"

Character enemylist[2][ENEMY_COUNT] = {{
	{
		.name = "Wood elemental baby",
		.health = 10,
		.elemental_type = ELEM_WOOD,
		.elements = {
			10,
			2,
			2,
			2,
			2
		},
		.weapon = &weapons_enemy[0],
		.skill = &skills_list[1]
	},

	{
		.name = "Wolf cub",
		.health = 7,
		.elemental_type = ELEM_WOOD,
		.elements = {
			4,
			2,
			2,
			2,
			2
		},
		.weapon = &weapons_enemy[0],
		.skill = &skills_list[0]
	},

	{
		.name = "Wild boar",
		.health = 10,
		.elemental_type = ELEM_WOOD,
		.elements = {
			5,
			2,
			2,
			2,
			2
		},
		.weapon = &weapons_enemy[0],
		.skill = &skills_list[0]
	},

	{
		.name = "Wolf mother",
		.health = 15,
		.elemental_type = ELEM_WOOD,
		.elements = {
			7,
			3,
			3,
			3,
			3,
		},
		.weapon = &weapons_enemy[0],
		.skill = &skills_list[0]
	},

	{
		.name = "Woodland critter",
		.health = 4,
		.elemental_type = ELEM_WOOD,
		.elements = {
			3,
			1,
			1,
			1,
			1
		},
		.weapon = &weapons_enemy[0],
		.skill = &skills_list[0]
	},

	{
		.name = "Bear",
		.health = 25,
		.elemental_type = ELEM_WOOD,
		.elements = {
			9,
			4,
			4,
			4,
			4
		},
		.weapon = &weapons_enemy[0],
		.skill = &skills_list[0]
	}
},

{
	{
		.name = "Wood elemental baby",
		.health = 10,
		.elemental_type = ELEM_WOOD,
		.elements = {
			5,
			2,
			2,
			2,
			2
		},
		.weapon = &weapons_enemy[0],
		.skill = &skills_list[1]
	},

	{
		.name = "Fire elemental baby",
		.health = 10,
		.elemental_type = ELEM_FIRE,
		.elements = {
			2,
			5,
			2,
			2,
			2
		},
		.weapon = &weapons_enemy[1],
		.skill = &skills_list[0]
	},

	{
		.name = "Earth elemental baby",
		.health = 10,
		.elemental_type = ELEM_EARTH,
		.elements = {
			2,
			2,
			5,
			2,
			2
		},
		.weapon = &weapons_list[0],
		.skill = &skills_list[0]
	},

	{
		.name = "Metal elemental baby",
		.health = 10,
		.elemental_type = ELEM_METAL,
		.elements = {
			2,
			2,
			2,
			5,
			2
		},
		.weapon = &weapons_list[0],
		.skill = &skills_list[0]
	},

	{
		.name = "Water elemental baby",
		.health = 10,
		.elemental_type = ELEM_WATER,
		.elements = {
			2,
			2,
			2,
			2,
			5
		},
		.weapon = &weapons_list[0],
		.skill = &skills_list[0]
	},

	{
		.name = "Touho the dog",
		.health = 99,
		.elemental_type = ELEM_EARTH,
		.elements = {
			99,
			99,
			99,
			99,
			99
		},
		.weapon = &weapons_enemy[2],
		.skill = &skills_list[0]}
}};
