#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include "locations.h"
#include "networking.h"
#include "globals.h"
#include "events.h"
#include "ui.h"
#include "weapons.h"
#include "skills.h"
#include "character.h"
#include "combat.h"

void set_player_location(Location* loc)
{
	player.location = loc;
	ncurs_location();
}

/* a modified set_player_location, doesn't draw description -> doesn't empty the screen*/
/* this function handles the keypress check: only returns if Y or N is pressed */
int set_player_loc_yesno() {
	wattron(game_win,A_BOLD);
	wprintw(game_win,_("Yes/No\n"));
	wattroff(game_win,A_BOLD);
	wrefresh(game_win);
	player.location = &loc_yesno;
	ncurs_commands();

	int loop = 1;
	while (loop) 
	{
		int keypress = getch();
		if (keypress == 'y') 
			return keypress;

		if (keypress == 'n') 
			return keypress;		
	} 

return 0;
}
void ac_dungeons()
{
	set_player_location(&loc_dungeons);
	player.dungeon_lvl = 0;
}

void ac_dungeons_enter() {
	set_player_location(&loc_dungeons_level1);
	player.dungeon_lvl = 1;
}

void ac_dungeons_action()
{
	if (player.action_points > 0)
	{
		player.action_points--;

		if (check_rnd_events() != 1) {/* return 1 => a random event occurred, don't fight */
			set_player_location(&loc_fight);
			create_enemy();
			ncurs_log_sysmsg(_("You encounter a %s!"), enemy.name);

			ncurs_fightinfo(&player, 0);
			ncurs_fightinfo(&enemy, 3);
		}
	}
	else
	{
		ncurs_log_sysmsg(_("You feel too tired to fight"));
	}
}

/* helper functions so I don't need to rewrite that much code */
void ac_fight_0() {
	use_skill(0);
}

void ac_fight_1() {
	use_skill(1);
}

void ac_fight_2() {
	use_skill(2);
}

void ac_fight_3() {
	use_skill(3);
}

void ac_dungeons_glow()
{
	set_player_location(&loc_shrine);
}

void ac_shrine_heal_1()
{
	if (player.health < player.max_health)
	{
		if (player.money > 0)
		{
			player.health++;
			player.money--;
			ncurs_log_sysmsg(_("%s was healed for 1 hitpoint"),player.name);
			ncurs_modal_msg(_("\nYou flip the coin into the pond, wishing for world peace."));
		}
		else
		{
			ncurs_modal_msg(_("\nYou search your pockets for coin, but find none."));
		}
	}
	else /* hp full already */
	{
		player.money--;
		ncurs_modal_msg(_("\nYou flip a coin in the pond.\nA crow catches the coin mid-air. Damn.."));
		ncurs_log_sysmsg(_("\nA crow stole a coin from %s"),player.name);
	}
}

void ac_shrine_heal_all()
{
	if (player.health < player.max_health)
	{
		if (player.money >= 5)
		{
			player.health += 10;
			if (player.health > player.max_health)
				player.health = player.max_health;
			player.money -= 5;
			ncurs_log_sysmsg(_("%s was healed for 10 hitpoints"),player.name);
			ncurs_modal_msg(_("\nYou flip 5 coins into the pond, wishing for world peace."));
		}
		else
		{
			ncurs_modal_msg(_("\nYou search your pockets for coin, but find none."));
		}
	}
	else /* hp full already */
	{
		player.money--;
		ncurs_log_sysmsg(_("A crow stole a coin from %s"),player.name);
		ncurs_modal_msg(_("\nYou flip a coin in the pond.\nA crow catches the coin mid-air. Damn.."));
	}
}

void ac_list_players()
{
	// TODO fetch player list
	ncurs_modal_msg(_("The wind is howling in the empty streets of this god forsaken town. You are all alone here."));
	set_player_location(&loc_town);
}

void ac_view_stats()
{
	// It looks ugly here, but correct ingame
	werase(game_win);
	wrefresh(game_win);
	wprintw(game_win,"%8s: %10s\n", _("Name"), player.name);
	wprintw(game_win,"%8s: %10d\n", _("Stamina"), player.action_points);
	wprintw(game_win,"%8s: %10d\n", _("XP"), player.experience);
	wprintw(game_win,"%8s: %10d\n", _("Money"), player.money);
	wprintw(game_win,"%8s: %7d/%2d\n", _("Health"), player.health,player.max_health);
	wattron(game_win,A_BOLD);
	wprintw(game_win,"\n%s:\n", _("Elements"));
	wattroff(game_win,A_BOLD);
	for (int i = 0; i < ELEM_COUNT; i++)
	{
		wprintw(game_win,"%8s: %10d\n", element_names[i], player.elements[i]);
	}
	wprintw(game_win,"\n");
	
	wprintw(game_win,"%8s: %10s (%s)\n", _("Weapon"), player.weapon->name,element_names[player.weapon->dmg_type]);
	for (int i = 0; i < 4; i++)
	{
		wprintw(game_win,"%6s %d: %10s\n", _("Skill"), i+1, player.skill[i]->name);
	}

	wrefresh(game_win);

	/* if stats are viewed during a fight, getch() and display char info again */
	/* TODO: player and enemy hardcoded to 0 and 3 */
	if (player.location == &loc_fight) {
		wprintw(game_win,"\n\n<MORE>");
		wrefresh(game_win);
		todd_getchar(NULL);
		werase(game_win);
		wrefresh(game_win);
		ncurs_fightinfo(&player, 0);
		ncurs_fightinfo(&enemy, 3);
	}
}

void ac_tavern()
{
	set_player_location(&loc_tavern);
}

void ac_tavern_bartender()
{
	set_player_location(&loc_tavern_bartender);
}

void ac_tavern_room()
{
	werase(game_win);
	wprintw(game_win,_("TODO: add player location to the database\n"));
	wprintw(game_win,_("TODO: pay for the room\n"));
	wprintw(game_win,_("TODO: possible random events: robbers / prostitutes\n"));
	wprintw(game_win,"\n");

	wprintw(game_win,_("Would you like to rent a room?\n"));
	wrefresh(game_win);

	/* TODO: testing yesno -dialog, make this better if need be */
	int keypress = set_player_loc_yesno();
	if (keypress == 'y') 
	{
		/* TODO: find out why modal_msg doesn't work here */
		wprintw(game_win,_("You leave your weapon by your bed and go to sleep..\n"));
		wprintw(game_win,_("Continue...\n"));
		wrefresh(game_win);
		getch();
		/* TODO: set_player_location wclears game_win, does it matter? */
		/* you could set_player_location in save_player_data (move the bard thing earlier */
		/* etc etc */
		set_player_location(&loc_room_offline);
		playing = false;
	}

	if (keypress == 'n') 
	{
	ncurs_modal_msg(_("Don't waste my time again!"));
	set_player_location(&loc_tavern_bartender);
	} 
}

void ac_tavern_info()
{
	/* TODO:
	information about
	1. other players
	2. stuff in the world (tutorial/help: bartender knows everything!)
	3. random stuff
	*/
	werase(game_win);
	ncurs_modal_msg("TODO: Implement\n");
}

void ac_warena()
{
	set_player_location(&loc_warena);
}

void ac_warena_skills()
{
	/* The function is exactly the same than in shop_buy, could these be combined? */
	wprintw(game_win,"\n%s\n", _("Bren can teach you one of these skills:"));
	int selection = ncurs_listselect(&(skills_list[0].name), sizeof(Skills), 0, SKILLS_COUNT);
	if (selection == 'x')
		ncurs_modal_msg(_("Your loss, buddy!"));
	else
	if (selection >= 'a' && selection < ('a' + SKILLS_COUNT))
	{
		/* slot contains 'x' on cancel/nevermind, a number otherwise */
		int slot = check_for_skill_slots(selection);
		/* check_for_skill_slots should check for valid input and return a number */
		if (slot == 'x')
			ncurs_modal_msg("%s",_("Your loss, buddy!\n"));
		else {
			/* listselect has returned selection as a letter, convert to a number */
			selection = selection - 'a';
	                player.skill[slot] = &skills_list[selection];
        	        ncurs_skills();
			ncurs_log_sysmsg(_("%s learned a new skill: %s"), player.name, player.skill[slot]->name);
			ncurs_modal_msg(_("YOU JUST LEARNED: %s"), player.skill[slot]->name);
		}
	}
}	

void ac_shop()
{
	set_player_location(&loc_shop);
}

void ac_shop_buy()
{
	werase(command_win);
	wrefresh(command_win);
	wprintw(game_win,"\n%s\n", _("\nThe poor man is selling these items:\n\n"));
	wattron(game_win, A_BOLD);
	wprintw(game_win,"Weapon\t\t\tPrice\n");
	wattroff(game_win, A_BOLD);
	wprintw(game_win,"\n");

 	int selection = ncurs_listselect(&(weapons_list[0].name), sizeof(Weapons), (void*)&(weapons_list[0].price) - (void*)&(weapons_list[0].name), WEAPON_COUNT);
	if (selection == 'x')
		ncurs_modal_msg(_("May the gods curse you for wasting my time, mutters the old man."));
	else
        if (selection >= 'a' && selection < ('a' + WEAPON_COUNT)) 
	{ 	

		/* Shop differs from warrior arena: It uses a hack to remove "Bare hands" from weapons listing.
		This here selection += 1; adjusts the numbers so the player keypress is in sync with what is printed in listselect */
		selection -= 'a';
		/* see if player has the money for it */
		if (player.money >= weapons_list[selection].price) {
			player.weapon = &weapons_list[selection];
			player.money = player.money - weapons_list[selection].price;
			ncurs_log_sysmsg(_("%s bought %s for %d"),player.name,player.weapon->name,player.weapon->price);
			ncurs_modal_msg(_("YOU JUST BOUGHT: %s"), player.weapon->name);
			ncurs_skills(); /* update Weapon & Skills -listing */
		}
		else
		{
			ncurs_modal_msg(_("Come back when you have the money for it!"));
		}

	}
}

void ac_shop_sell()
{
	ncurs_modal_msg("The poor man has no coins to buy anything from you.\nyou have: %s",player.weapon->name);
}

void ac_messageboard()
{
	set_player_location(&loc_messageboard);
}

void ac_messageboard_view()
{
	wclear(game_win);
	PGresult *res;
	res = PQexecPrepared(conn, "view_messageboard", 0, NULL, NULL, NULL, 0);
	if (PQresultStatus(res) == PGRES_TUPLES_OK)
	{
		int row_count = PQntuples(res);
		// int col_count = PQnfields(res); /* NOT USED ANYWHERE, IS THIS NEEDED? */
		for (int i = 0; i < row_count; i++)
		{
			wprintw(game_win, "%s // ", PQgetvalue(res, i, 0));
			wprintw(game_win, "%s\n", PQgetvalue(res, i, 1));
			wprintw(game_win, "%s ", PQgetvalue(res, i, 2));
			wprintw(game_win, "\n\n");
		}
	}
	wrefresh(game_win);
	PQclear(res);
}

extern char* itoa(int i);
void ac_messageboard_write()
{
ncurs_bold_input(1);
/* echo, noecho and wgetnstr(input_win) enable users to see what they write */
	int len = 80;
	char *line = malloc(len); // more dynamic memory allocation would be nice
	echo();
	if (wgetnstr(input_win,line, len) != ERR) // TODO a better way to get input
	{
		char *player_id = itoa(player.id);
		const char *params[2] = {player_id, line};
		PGresult *res;
		res = PQexecPrepared(conn, "write_to_messageboard", 2, params, NULL, NULL, 0);
		if (PQresultStatus(res) != PGRES_COMMAND_OK)
		{
			syslog(LOG_WARNING, "Messageboard write failed: %s", PQresultErrorMessage(res));
		}
		PQclear(res);
		free(player_id);
	}
	free(line);
	noecho();
	ncurs_bold_input(0);
	
	werase(game_win);
	ncurs_modal_msg(_("Your message has been written.\nYou start to wonder if it was worth it."));
}

void ac_return_to_town()
{
	set_player_location(&loc_town);
}


void ac_quit()
{
	werase(game_win);
	/* using modal_msg here would force the user to waste a keypress */
	wprintw(game_win,_("You leave the town, wondering what treasures you left behind in the dungeons\n"));
	wrefresh(game_win);
	playing = false;
}

// wtf is this doing here? ncurs_* functions should probably be in ui.c?
void ncurs_chat() {
	char *line = NULL;
	size_t len = 0;
	if (todd_getline(&line, &len))
		send_chatmsg(line, len);
	free(line);
}

