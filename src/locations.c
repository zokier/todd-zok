#include <stdio.h>

#include "actions.h"
#include "location.h"

Location loc_town = {
	"You wander around the desolate town.\nYou begin to wonder why you even bothered coming here.",
	7,
	{
		{
			'd',
			"",
			"Dungeons",
			&ac_dungeons
		},
		{
			'l',
			"",
			"List players",
			&ac_list_players
		},
		{
			'v',
			"",
			"View stats",
			&ac_view_stats
		},
		{
			't',
			"",
			"Tavern",
			&ac_tavern
		},
		{
			's',
			"",
			"Shop",
			&ac_shop
		},
		{
			'm',
			"",
			"Messageboard",
			&ac_messageboard
		},
		{
			'q',
			"",
			"Quit",
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
			"Look for ",
			"Action",
			&ac_dungeons_action
		},
		{
			'g',
			"Examine the ",
			"Glow",
			&ac_dungeons_glow
		},
		{
			'r',
			"",
			"Return to town",
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
			"",
			"Yes",
			&ac_ev_oldman_help
		},
		{
			'n',
			"",
			"No",
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
			"",
			"Yell",
			&ac_tavern_shout
		},
		{
			'r',
			"",
			"Return to town",
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
			"",
			"Buy items",
			&ac_shop_buy
		},
		{
			's',
			"",
			"Sell items",
			&ac_shop_sell
		},
		{
			'r',
			"",
			"Return to town",
			&ac_return_to_town
		}
	}
};

Location loc_fight = {
	"",
	3,
	{
		{
			'r',
			"",
			"Run",
			&ac_return_to_town
		},
		{
			'0',
			"",
			NULL,
			&ac_fight_0
		},

		{
			'1',
			"",
			NULL,
			&ac_fight_1
		}
	}
};

Location loc_shrine = {
	"Inside the ruins you find a peaceful shrine.\n\nIn the middle of it you see a largish crystal faintly glowing red. Curiously the crystal has a neat slot in it, perfectly sized for a coin.",
	3,
	{
		{
			'i',
			"",
			"Insert a coin",
			&ac_shrine_heal_1
		},
		{
			'a',
			"Insert ",
			"All the coins",
			&ac_shrine_heal_all
		},
		{
			'r',
			"",
			"Return to town",
			&ac_return_to_town
		}
	}
};

Location loc_messageboard = {
	"There is a large messageboard in the middle of the town square. It is plastered with layers of old notes. Most of them are weathered completely unintelligible. Beside the board is a small table with a pen and a stack of empty papers.",
	3,
	{
		{
			'v',
			"",
			"View messages",
			&ac_messageboard_view
		},
		{
			'w',
			"",
			"Write a message",
			&ac_messageboard_write
		},
		{
			'r',
			"",
			"Return to town",
			&ac_return_to_town
		}
	}
};
