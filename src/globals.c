/* globals.c - shared stuff with ToDD and dailybot */
#include "globals.h"
Character player;

Party player_party = {
	.id = 0,	// NOTE that currently 0 means no party
	.name = "",	// "" or NULL or what? must be printable in view stats
	.characters = {
		&player,
		NULL,
		NULL
	}
};
