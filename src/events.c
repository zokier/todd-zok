#include <stdio.h>
#include <stdbool.h>
#include <time.h> /* for random number generator */
#include <stdlib.h>

#include "ui.h"
#include "events.h"
#include "locations.h"

extern void set_player_location(Location* loc);

int check_rnd_events() {

        int i = rand() % 1000;
        /* calculate probabilities to random events and then switch to the proper function */
        /* this part is only meant to make switching */

	int prob_max = 0;
	#define PROB(x) (i < (prob_max += x))

	
        // TODO: write the rest of the events, adjust probabilities
	/* events are based on dungeon levels. Some events can occur in many levels */
	switch(player.dungeon_lvl) {
		case 0: {
		        if (PROB(10)) /* ==> chance is 10 out of 1000 */
		                return ev_old_man();
			break;
			}
		case 1: {
		        if (PROB(10)) /* ==> chance is 10 out of 1000 */
		                return ev_bag_of_gold();
			break;
			}
		default:
			break;
		}
	#undef PROB
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
        ncurs_modal_msg("Thank you kind sir! Here take this for your trouble.\n\n"
                        "The old man shoves a purse in your hands.\n");
        ncurs_log_sysmsg("You gain 100 gold!");
        player.money += 100;
        set_player_location(&loc_dungeons);
}

void ac_ev_oldman_nohelp()
{
        ncurs_log_chatmsg("May the gods curse you.", "Old man"); // just for fun
        player.action_points++;
        set_player_location(&loc_dungeons);
}



/* A BAG OF GOLD */
int ev_bag_of_gold()
{
	ncurs_modal_msg("You found a bag of gold with 100 gold pieces in it!\n");
	player.money = player.money +100;
	return 1;
}
