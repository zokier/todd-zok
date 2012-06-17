#ifndef SKILLS_H
#define SKILLS_H

#include "element.h"

#define SKILLS_COUNT 3

typedef struct Skills Skills;
struct Skills {
	char name[20];
	// TODO add more stats
	Element dmg_type;
	int damage;
	int ap_cost;
};

#endif //SKILLS_H
