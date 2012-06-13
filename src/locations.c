#include <stdio.h>

#include "actions.h"
#include "location.h"

// ansi control code for underlining, and reverting back to normal

Location loc_town = {
	"You wander around the desolate town.\nYou begin to wonder why you even bothered coming here.",
	6,
	{
		{
			'd',
			"(D)ungeons",
			&ac_dungeons
		},
		{
			'l',
			"(L)ist players",
			&ac_list_players
		},
		{
			'v',
			"(V)iew stats",
			&ac_view_stats
		},
		{
			't',
			"(T)avern",
			&ac_tavern
		},
		{
			's',
			"(S)hop",
			&ac_shop
		},
		{
			'q',
			"(Q)uit",
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
	"As you ponder how large bugs must be to threaten grown  men, you notice faint, strangely compelling, reddish\nglow emanating from nearby ruins.",
	3,
	{
		{
			'a',
			"Look for (A)ction",
			&ac_dungeons_action
		},
		{
			'g',
			"Examine the (G)low",
			&ac_dungeons_glow
		},
		{
			'r',
			"(R)eturn to town",
			&ac_return_to_town
		}
	}
};

Location loc_ev_oldman = {
	"An old man is wandering along the path, looking confused and worried.\n"
	"-It seems I have managed to lose my way to the town.\nWould you be so kind to escort me there?\n"
	"\nHelp the old man?",
	2,
	{
		{
			'y',
			"(Y)es",
			&ac_ev_oldman_help
		},
		{
			'n',
			"(N)o",
			&ac_ev_oldman_nohelp
		}
	}
};

Location loc_tavern = {
	"The tavern is eerlily quiet.\n\nYou thought you saw something move in the shadows, but\nfound nothing. What sort of madness leads a man to builda fine tavern like this, and then abadon it?",
	2,
	{
		{
			'y',
			"(Y)ell",
			&ac_tavern_shout
		},
		{
			'r',
			"(R)eturn to town",
			&ac_return_to_town
		}
	}
};

Location loc_shop = {
	"You see a scrawny man in one of the shacks that make up most of the town. The man introduces himeself as a\nshopkeeper, but this not like any shop you've ever seen.",
	3,
	{
		{
			'b',
			"(B)uy items",
			&ac_shop_buy
		},
		{
			's',
			"(S)ell items",
			&ac_shop_sell
		},
		{
			'r',
			"(R)eturn to town",
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
			"(F)ight",
			&ac_fight_fight
		},
		{
			'r',
			"(R)un",
			&ac_return_to_town
		}
	}
};

Location loc_shrine = {
	"Inside the ruins you find a peaceful shrine.\n\nIn the middle of it you see a largish crystal faintly glowing red. Curiously the crystal has a neat slot in it, perfectly sized for a coin.",
	3,
	{
		{
			'i',
			"(I)nsert a coin",
			&ac_shrine_heal_1
		},
		{
			'a',
			"Insert (A)ll the coins",
			&ac_shrine_heal_all
		},
		{
			'r',
			"(R)eturn to town",
			&ac_return_to_town
		}
	}
};
