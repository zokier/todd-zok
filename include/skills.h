#ifndef SKILLS_H
#define SKILLS_H

#include "element.h"

#define SKILLS_COUNT 4 // this MUST include the "Unused" skill as well


typedef struct Skills Skills;
struct Skills {
	int index;
	char *name;
	// TODO add more stats
	Element dmg_type;
	int damage;
	int ap_cost;
};

int check_for_skill_slots(int skillnumber);


#endif //SKILLS_H

