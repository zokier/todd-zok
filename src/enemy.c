#include "enemy.h"

extern Skills skills_list[];
extern Weapons weapons_list[];

Enemy enemylist[ENEMY_NR] = {
{"Wood elemental baby",10,TYPE_WOOD,5,2,2,2,2,&weapons_list[1],&skills_list[1]},
{"Fire elemental baby",10,TYPE_FIRE,2,5,2,2,2,&weapons_list[0],&skills_list[0]},
{"Earth elemental baby",10,TYPE_EARTH,2,2,5,2,2,&weapons_list[0],&skills_list[0]},
{"Metal elemental baby",10,TYPE_METAL,2,2,2,5,2,&weapons_list[0],&skills_list[0]},
{"Water elemental baby",10,TYPE_WATER,2,2,2,2,5,&weapons_list[0],&skills_list[0]},
{"Touho the dog",99,TYPE_EARTH,99,99,99,99,99,&weapons_list[3],&skills_list[0]}
};
