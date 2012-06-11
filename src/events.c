#include <stdio.h>
#include <stdbool.h>

#include "events.h"
#include "input.h"
#include "globals.h"
#include "ui.h"
int ev_old_man()
{
	wclear(gamew);
	wprintw(gamew,"An old man is wandering along the path, looking confused and worried.\n\n");
	wprintw(gamew,"-It seems I have managed to lose my way to the town.\nWould you be so kind to escort me there?\n\n");

	wprintw(gamew,"\nHelp the old man? (y/n)");
	wrefresh(gamew);
	char c;
	while (true) 
	{
		if (!todd_getchar(&c))
		{
			return 0;
		}
		if (c == 'n')
		{
			wclear(gamew);
			wattron(gamew,A_BOLD);
			wprintw(gamew,"May the gods curse you, mutters the old man.\n\n");
	
			wprintw(gamew,"<MORE>");
			wattroff(gamew,A_BOLD);
			wrefresh(gamew);
			getch();
			return 0;
		}
		else if (c == 'y')
		{
			wclear(gamew);
			wprintw(gamew,"Thank you kind sir! Here take this for your trouble.\n\n");
			wprintw(gamew,"The old man shoves a purse in your hands.");
			wattron(gamew,A_BOLD);
			wprintw(gamew,"\n\nYou gain 100 gold!");
			wattroff(gamew,A_BOLD);
			wrefresh(gamew);
			player.money += 100;
			return 1;
		}

	}
}

