#ifndef ENEMY_H
#define ENEMY_H

#define ENEMY_NR 6
typedef struct Enemy Enemy;
struct Enemy {
	char *name;
        // TODO add more stats
        int wood;
        int fire;
        int earth;
        int metal;
        int water;
};

#endif //ENEMY_H
