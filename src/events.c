#include <stdio.h>
#include <stdbool.h>
#include <time.h> /* for random number generator */
#include <stdlib.h>
#include <string.h>

#include "ui.h"
#include "events.h"
#include "locations.h"

extern void set_player_location(Location* loc);

int prob_max = 0;
#define PROB(x) (i < (prob_max += x))
// PROB(1) means 1 out 1000. PROB(500) means 500/1000 and so on


int check_rnd_events() {
// TODO: write the rest of the events, adjust probabilities
	
        /* calculate probabilities to random events and then switch to the proper function */
        /* this part is only meant to make switching */

	int i = rand() % 1000;

	/* events are based on dungeon levels. Some events can occur in many levels */
	switch(player.dungeon_lvl) {
		case 0: {
		        if (PROB(10)) /* ==> chance is 10 out of 1000 */
		                return ev_found_item();

		        if (PROB(10)) /* ==> chance is 10 out of 1000 */
		                return ev_old_man();
			break;
			}
		case 1: {
		        if (PROB(10)) /* ==> chance is 10 out of 1000 */
		                return ev_bag_of_gold();
			break;
			}

		case 2: {
			break;
			}

		default:
			break;
		}
	/* if there's no random event, return 0. All random events should return 1; */
	return 0;
}


/* OLD MAN */
int ev_old_man()
{
	set_player_location(&loc_ev_oldman);
	return 1;
}

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

void ac_ev_oldman_help()
{
        ncurs_modal_msg(_("Thank you kind sir! Here take this for your trouble.\n\nThe old man shoves a purse in your hands."));
        ncurs_log_sysmsg(_("You gain 100 gold!"));
        player.money += 100;
        set_player_location(&loc_dungeons);
}

void ac_ev_oldman_nohelp()
{
        ncurs_log_chatmsg(_("May the gods curse you."), "Old man"); // just for fun
        player.action_points++;
        set_player_location(&loc_dungeons);
}



/* A BAG OF GOLD */
int ev_bag_of_gold()
{
	ncurs_modal_msg(_("You find a bag of gold with 100 gold pieces in it!\n"));
	player.money = player.money +100;
	return 1;
}

/* ITEM FOUND */
int ev_found_item()
{
	prob_max = 0;
	int i = rand() % 1000;
	int item_found = 0;
	/* random item found: */
	char buffer[30] = "";
        if (PROB(500)) {
		strcpy(buffer,"NOTHING\n");
		}
	else	{ /* found a random item. TODO: check for ther stuff than weapons */
		i = 0;
		while (i == 0) /* don't accept bare hands, weapon 0 as an item found */
			i = rand() % WEAPON_COUNT;

		strcpy(buffer,weapons_list[i].name);
		item_found = 1;
	}

	werase(command_win);
	wrefresh(command_win);

	/* The drawing routine is done like this to add excitement */
	werase(game_win);
	wprintw(game_win,_("Buried under a pile of rocks, you find...\n"));
	wprintw(game_win,"<MORE>");
	wrefresh(game_win);
	todd_getchar(NULL);
	

	/* print out what you got */
	mvwprintw(game_win,1,0,buffer);
	
	if (item_found) { /* found an item */
	wprintw(game_win,_("\nFor some odd reason, you discard your %s and take the newly found %s\n"),player.weapon->name,buffer);
	wprintw(game_win,"\nEVENT TODO: randomize for more items once you have them\n");
	wprintw(game_win,"EVENT TODO: make a function that gives player a choice to discard the new weapon\n");
	player.weapon = &weapons_list[i];
	}
	wprintw(game_win,"\nContinue...");
	wrefresh(game_win);
	todd_getchar(NULL);

	set_player_location(player.location); /* redraw commands */
	return 1;
}

