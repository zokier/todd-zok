#ifndef SKILLS_H
#define SKILLS_H

/* damage types, declared here for a lack of a better place */
typedef enum Element Element;
enum Element
{
	ELEM_WOOD = 0,
	ELEM_FIRE,
	ELEM_EARTH,
	ELEM_METAL,
	ELEM_WATER,
};

#define SKILLS_COUNT 3

typedef struct Skills Skills;
struct Skills {
	char *name;
	// TODO add more stats
	int dmg_type;
	int damage;
	int ap_cost;
};

#endif //SKILLS_H
