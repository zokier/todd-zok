#include <stdio.h>

#include "actions.h"
#include "location.h"

Location loc_town = {
	"You wander around the desolate town.\nYou begin to wonder why you even bothered coming here.",
	8,
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
			'w',
			"",
			"Warrior arena",
			&ac_warena
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
	"As you ponder how large bugs must be to threaten grown  men, you notice faint, strangely compelling, reddish\nglow emanating from nearby ruins."
	"\n"
	"You may enter the dungeons or look for action in the surrounding forest.",
	4,
	{
		{
			'e',
			"",
			"Enter dungeons",
			&ac_dungeons_enter
		},

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

Location loc_dungeons_level1 = {
	"You enter the dungeons. There are dimly lit torches on the wall. You realize that other people have been here.\n"
	"\nTODO: Finish the description, add something else besides look for action\n",
	2,
	{
		{
			'a',
			"Look for ",
			"Action",
			&ac_dungeons_action
		},
		{
			'r',
			"",
			"Return to fresh air",
			&ac_dungeons
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

Location loc_warena = {
	"You enter Bren's Warrior arena.\n\nThe place is populated with warriors-to-be, training in all aspects of fighting.",
	2,
	{
		{
			'a',
			"",
			"Ask for a lesson",
			&ac_warena_skills
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
	6,
	{
		{
			'r',
			"",
			"Run",
			&ac_return_to_town
		},
		{
			'v',
			"",
			"View stats",
			&ac_view_stats
		},

		{
			'1',
			"",
			NULL,
			&ac_fight_0
		},

		{
			'2',
			"",
			"Press 1-4",
			&ac_fight_1
		},
		{
			'3',
			"",
			NULL,
			&ac_fight_2
		},

		{
			'4',
			"",
			NULL,
			&ac_fight_3
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

