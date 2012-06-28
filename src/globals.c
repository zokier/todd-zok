/* globals.c - shared stuff with ToDD and dailybot */
#include "globals.h"
Character player;

Party player_party = {
	.id = 0,
	.characters = {
		&player,
		NULL,
		NULL
	}
};
