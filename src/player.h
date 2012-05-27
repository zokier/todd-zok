#ifndef PLAYER_H
#define PLAYER_H

#include "location.h"

typedef struct Player Player;
struct Player {
	char *name;
	Location *location;
};

#endif //PLAYER_H
