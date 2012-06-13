#ifndef ENEMY_H
#define ENEMY_H

typedef struct Enemy Enemy;
struct Enemy {
	char *name;
	int health;
        // TODO add more stats
        int wood;
        int fire;
        int earth;
        int metal;
        int water;
};

#endif //ENEMY_H
