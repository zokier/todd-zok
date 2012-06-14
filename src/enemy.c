#include "enemy.h"

extern Skills skills_list[];
Enemy enemylist[ENEMY_NR] = {
{"Wood elemental baby",TYPE_WOOD,5,2,2,2,2,&skills_list[0]},
{"Fire elemental baby",TYPE_FIRE,2,5,2,2,2,&skills_list[0]},
{"Earth elemental baby",TYPE_EARTH,2,2,5,2,2,&skills_list[0]},
{"Metal elemental baby",TYPE_METAL,2,2,2,5,2,&skills_list[0]},
{"Water elemental baby",TYPE_WATER,2,2,2,2,5,&skills_list[0]},
{"Touho the dog",TYPE_WATER,99,99,99,99,99,&skills_list[0]}
};
