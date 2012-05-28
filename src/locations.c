#include <stdio.h>

#include "actions.h"
#include "location.h"

// ansi control code for underlining, and reverting back to normal
#define UNDERLINE(x) "\033[4m" x "\033[0m"

Location loc_town = {
	"You begin to wonder why you even bothered coming here as you wander around the desolate town.",
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
	"\t+----------------+\n"
	"\t| Beware of bugs |\n"
	"\t+----------------+\n"
	"\n"
	"As you ponder how large bugs must be to threaten grown men, you notice faint, strangely compelling, reddish glow emanating from nearby ruins.",
	3,
	{
		{
			'a',
			"Look for "UNDERLINE("a")"ction",
			&ac_dungeons_action
		},
		{
			'g',
			"Examine the "UNDERLINE("g")"low",
			&ac_dungeons_glow
		},
		{
			'r',
			UNDERLINE("R")"eturn to town",
			&ac_return_to_town
		}
	}
};

Location loc_tavern = {
	"The tavern is eerlily quiet. You thought you saw something move in the shadows, but found nothing. What sort of madness leads man to build a fine tavern like this, and then abadon it?",
	2,
	{
		{
			's',
			UNDERLINE("S")"hout",
			&ac_tavern_shout
		},
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

Location loc_fight = {
	"",
	2,
	{
		{
			'f',
			UNDERLINE("F")"ight",
			&ac_fight_fight
		},
		{
			'r',
			UNDERLINE("R")"un",
			&ac_return_to_town
		}
	}
};

Location loc_shrine = {
	"Inside the ruins you find a peaceful shrine. In the middle of it you see a largish crystal faintly glowing red. Curiously the crystal has a neat slot in it, perfectly sized for a coin.",
	3,
	{
		{
			'i',
			UNDERLINE("I")"nsert a coin",
			&ac_shrine_heal_1
		},
		{
			'a',
			"Insert "UNDERLINE("a")"ll the coins",
			&ac_shrine_heal_all
		},
		{
			'r',
			UNDERLINE("R")"eturn to town",
			&ac_return_to_town
		}
	}
};
