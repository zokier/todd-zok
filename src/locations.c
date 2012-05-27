#include <stdio.h>

#include "actions.h"
#include "location.h"

#define UNDERLINE(x) "\033[4m" x "\033[0m"

Location loc_town = {
	"You begin to wonder why you even bothered coming here when as you wander around the desolate town.",
	6,
	{
		{
			'd',
			UNDERLINE("D")"ungeons",
			&ac_dungeons
		},
		{
			'l',
			UNDERLINE("L")"ist players",
			&ac_list_players
		},
		{
			'v',
			UNDERLINE("V")"iew stats",
			&ac_view_stats
		},
		{
			't',
			UNDERLINE("T")"avern",
			&ac_tavern
		},
		{
			's',
			UNDERLINE("S")"hop",
			&ac_shop
		},
		{
			'q',
			UNDERLINE("Q")"uit",
			&ac_quit
		}

	}
};

Location loc_dungeons = {
	"You follow a road that is supposed to lead to dungeons. At the end of the road you find a barred gateway with a sign:\n"
	"\n"
	"\t+--------------+\n"
	"\t| Out of order |\n"
	"\t+--------------+\n"
	"\n",
	1,
	{
		{
			'r',
			UNDERLINE("R")"eturn to town",
			&ac_return_to_town
		}
	}
};

Location loc_tavern = {
	"The tavern is eerlily quiet. You thought you saw something move in the shadows, but found nothing. What sort of madness leads man to build a fine tavern like this, and then abadon it?",
	1,
	{
		{
			'r',
			UNDERLINE("R")"eturn to town",
			&ac_return_to_town
		}
	}
};

Location loc_shop = {
	"You see a scrawny man in one of the shacks that make up most of the town. The man introduces himeself as a shopkeeper, but this not like any shop you've ever seen.",
	3,
	{
		{
			'b',
			UNDERLINE("B")"uy items",
			&ac_shop_buy
		},
		{
			's',
			UNDERLINE("S")"ell items",
			&ac_shop_sell
		},
		{
			'r',
			UNDERLINE("R")"eturn to town",
			&ac_return_to_town
		}
	}
};

