#ifndef SKILLS_H
#define SKILLS_H

/* damage types, declared here for a lack of a better place */
#define TYPE_WOOD 1
#define TYPE_FIRE 2
#define TYPE_EARTH 3
#define TYPE_METAL 4
#define TYPE_WATER 5

#define SKILLS_NR 3
typedef struct Skills Skills;
struct Skills {
        char *name;
        // TODO add more stats
        int dmg_type;
	int damage;
};

#endif //SKILLS_H
