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
			ncurs_log_sysmsg("You encounter a %s!", enemy.name);

			ncurs_fightinfo(&player, 0);
			ncurs_fightinfo(&enemy, 3);
		}
	}
	else
	{
		ncurs_log_sysmsg("You feel too tired to fight\n");
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
			ncurs_modal_msg("The coin disappears into the slot with a clink. The crystal brightens for a moment and you step away from it feeling slightly better.");
		}
		else
		{
			ncurs_modal_msg("You search your pockets for coin, but find none.");
		}
	}
	else
	{
		ncurs_modal_msg("You try to insert a coin into the slot but to your surprise the crystal repels your coin.");
	}
}

void ac_shrine_heal_all()
{
	int money = player.money;
	money -= player.max_health - player.health;
	player.health = player.max_health;
	if (money < 0)
	{
		// TODO print different messsage 
		player.health += money;
		money = 0;
	}
	player.money = money;
	ncurs_modal_msg("The crystal dazes you with blindingly bright light. As you regain control, you notice that all your wounds have mended.");
	set_player_location(&loc_dungeons);
}

void ac_list_players()
{
	// TODO fetch player list
	ncurs_modal_msg("The wind is howling in the empty streets of this god forsaken town. You are all alone here.");
	set_player_location(&loc_town);
}

void ac_view_stats()
{
	// TODO prettify
	werase(game_win);
	wrefresh(game_win);
	wprintw(game_win,"Name:         %s\n", player.name);
	wprintw(game_win,"Stamina / AP: %d\n", player.action_points);
	wprintw(game_win,"XP:           %d\n", player.experience);
	wprintw(game_win,"Money:        %d\n",player.money);
	wprintw(game_win,"Health:	%d/%d\n",player.health,player.max_health);
	wattron(game_win,A_BOLD);
	wprintw(game_win,"\nElements:\n");
	wattroff(game_win,A_BOLD);
	wprintw(game_win,"Wood:         %d\n",player.elements[ELEM_WOOD]);
	wprintw(game_win,"Fire:         %d\n",player.elements[ELEM_FIRE]);
	wprintw(game_win,"Earth:        %d\n",player.elements[ELEM_EARTH]);
	wprintw(game_win,"Metal:        %d\n",player.elements[ELEM_METAL]);
	wprintw(game_win,"Water:        %d\n",player.elements[ELEM_WATER]);
	wrefresh(game_win);
}

void ac_tavern()
{
	set_player_location(&loc_tavern);
}

void ac_tavern_shout()
{
	puts("What do you want to yell?");
	// TODO line input widget
	int len = 80;
	char *line = malloc(len); // more dynamic memory allocation would be nice
	if (getnstr(line, len) != ERR)
	{
		send_chatmsg(line, strlen(line));
		ncurs_log_chatmsg(line, player.name);
	}
	free(line);
}

void ac_warena()
{
	set_player_location(&loc_warena);
}

void ac_warena_skills()
{
	/* The function is exactly the same than in shop_buy, could these be combined? */
	wprintw(game_win,"\nBren can teach you one of these skills:\n");
	int selection = ncurs_listselect(&(skills_list[0].name), sizeof(Skills), SKILLS_COUNT);
	if (selection > 0)
	{
		player.skill[0] = &skills_list[selection];
		ncurs_skills();

		ncurs_log_sysmsg("%s learned a new skill: %s\n",player.name, player.skill[0]->name);
		ncurs_modal_msg("YOU JUST LEARNED: %s",player.skill[0]->name);
	}
	else
	{
		ncurs_modal_msg("Your loss, buddy!\n");
	}
}	

void ac_shop()
{
	set_player_location(&loc_shop);
}

void ac_shop_buy()
{
	wprintw(game_win,"\nThe poor man is selling these items:\n\n");
	int selection = ncurs_listselect(&(weapons_list[0].name), sizeof(Weapons), WEAPON_COUNT);
	if (selection > 0)
	{
		/* see if player has the money for it */
		if (player.money >= weapons_list[selection].price) {
			player.weapon = &weapons_list[selection];
			ncurs_log_sysmsg("%s bought %s\n for %d",player.name,player.weapon->name,player.weapon->price);
			ncurs_modal_msg("YOU JUST BOUGHT: %s",player.weapon->name);
		}
		else
		{
			ncurs_modal_msg("Come back when you have the money for it!\n");
		}

	}
	else
	{
		ncurs_modal_msg("May the gods curse you for wasting my time, mutters the old man.");
	}
}

void ac_shop_sell()
{
	ncurs_modal_msg("The poor man has no coins to buy anything from you.\n"
	                "you have: %s",player.weapon->name);
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
		int col_count = PQnfields(res);
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
	ncurs_modal_msg("\n\nYour message has been written.\nYou start to wonder if it was worth it.\n");
}

void ac_return_to_town()
{
	set_player_location(&loc_town);
}


void ac_quit()
{
	werase(game_win);
	/* using modal_msg here would force the user to waste a keypress */
//	ncurs_modal_msg("You leave the town, wondering what treasures you left behind in the dungeons\n");
	wprintw(game_win,"You leave the town, wondering what treasures you left behind in the dungeons\n");
	wrefresh(game_win);
	playing = false;
}

void ncurs_chat(Character player) {
int len = 80;
char *line = malloc(len); // more dynamic memory allocation would be nice
echo();
/* TODO: make bolding a specified window title a function */
ncurs_bold_input(1);

if (wgetnstr(input_win,line, len) != ERR) // TODO a better way to get input
	ncurs_log_chatmsg(line,player.name); 
noecho();
wrefresh(input_win);
/* draw the title again, this time with no bolding */
ncurs_bold_input(0);

}

