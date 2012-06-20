#include <string.h>
#include "skills.h"
#include "globals.h"

Skills unused_skill = { "Unused",0,0,0 };
Skills skills_list[SKILLS_COUNT] = {

	{
		"Basic Attack",	/* name */
		ELEM_EARTH,	/* element */
		0,		/* damage */
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
		ELEM_METAL,
		5,
		2
	}

};

int check_for_skill_slots(int skillnumber) {
	wprintw(game_win, "Select skill slot:\n");
	char *slots[4] = {
		"Skill slot 1",
		"Skill slot 2",
		"Skill slot 3",
		"Skill slot 4"
	};
	int slot_number = ncurs_listselect(slots, sizeof(char*), 0, 4);
	if (slot_number >= 0)
	{
		player.skill[slot_number] = &skills_list[skillnumber];
		ncurs_skills();
	}
	return slot_number;

}

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
