#include "skills.h"

Skills unused_skill = { "Unused",0,0,0 };
Skills skills_list[SKILLS_COUNT] = {

	{
		"Basic Attack",	/* name */
		ELEM_EARTH,	/* element */
		3,		/* damage */
		1		/* ap cost */
	},

	{
		"Wood Attack",
		ELEM_WOOD,
		3,
		1
	},

	{
		"Heavy attack",
		ELEM_EARTH,
		5,
		2
	}

};

/********************
Ideas for skills:

skill proficiency: 
elements!

weapon proficiency:
for every succesful skill / weapon use, you get +1 proficiency for that skill / weapon when damage is done, do rand() % 100. If number is 
smaller than proficiency, get SUPER STRIKE, double damage etc Basically, chance for a SUPER STRIKE increases when proficiency increases

iniative:
who gets to execute attack first

	attack skills
things to consider:
speed? ap cost? element manipulation?
	* wait out this turn, attack triple damage next turn

defensive skills
	* raises elemental stats temporarily
	* shifts points from 1 element to another
	* removes enemy elemental bonus

manipulating skills
element manipulation
fight mechanics manipulation: 
	* enemy can't attack next turn ("stun")
	* iniative drops, disable a skill
	* double/half ap cost for skills
	* randomize stuff


********************/
