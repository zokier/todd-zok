#include <stdio.h>
#include <stdbool.h>

#include "events.h"
#include "input.h"
#include "globals.h"

int ev_old_man()
{
	// Should this be a "location"?
	puts("An old man is wandering along the path, looking confused and worried.");
	puts("\t\"It seems I have managed to lose my way to the town. Would you be so kind to escort me there?\"");
	char c;
	while (true) 
	{
		puts("\nHelp the old man? (y/n)");
		if (!todd_getchar(&c))
		{
			return 0;
		}
		if (c == 'n')
		{
			return 0;
		}
		else if (c == 'y')
		{
			puts("Thank you kind sir! Here take this for your trouble.");
			puts("The old man shoves a purse in your hands. You gain 100 gold!");
			player.money += 100;
			return 1;
		}
	}
}

