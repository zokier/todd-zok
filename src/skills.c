#include <string.h>
#include "globals.h"
#include "ui.h"
#include "skills.h"

Skills skills_list[SKILLS_COUNT] = {
	{
		0,
		"Basic Attack",	/* name */
		ELEM_EARTH,	/* element */
		0,		/* damage */
		1		/* ap cost */
	},

	{
		1,
		"Wood Attack",
		ELEM_WOOD,
		3,
		1
	},

	{
		2,
		"Heavy attack",
		ELEM_METAL,
		5,
		2
	},

	{	// Unused skill, should never be looped anywhere
		999,
		"Unused",
		0,
		0,
		0		
	}
};

int check_for_skill_slots(int skillnumber) {
	wprintw(game_win, _("\nSelect skill slot:\n"));
	char *slots[4] = {
		"Skill slot 1",
		"Skill slot 2",
		"Skill slot 3",
		"Skill slot 4"
	};
	int slot_number = ncurs_listselect(slots, sizeof(char*), 0, 4);
	return slot_number;
}

