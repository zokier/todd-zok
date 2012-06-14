#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include "player.h"
#include "locations.h"
#include "enemy.h"
#include "networking.h"
#include "globals.h"
#include "events.h"
#include "ui.h"
#include "weapons.h"
#include "skills.h"

// globals are nasty?
Enemy enemy;
WINDOW *fight_youw;
WINDOW *fight_enemyw;

extern Weapons weapons[];
extern Enemy enemylist[];
extern Skills skills_list[];
int check_rnd_events() {

	int i = rand() % 1000;
	/* calculate probabilities to random events and then switch to the proper function */
	/* this part is only meant to make switching */

#define PROB(x) (i < (prob_max += x))
    int prob_max = 0;
    // TODO: write the rest of the events, adjust probabilities
	if (PROB(10)) /* ==> chance is 10 out of 1000 */
		return ev_old_man();

	if (PROB(10)) /* ==> chance is 10 out of 1000 */
		return ev_old_man();

	if (PROB(10)) /* ==> chance is 10 out of 1000 */
		return ev_old_man();

	if (PROB(2)) /* ==> chance is 2 out of 1000 */
		return ev_old_man();
#undef PROB
	/* if there's no random event, return 0. All random events should return 1; */
	return 0;
}

void create_enemy()
{
/* randomly choose an enemy from enemylist */
int random_enemy = rand() % ENEMY_NR;
memcpy(&enemy,&enemylist[random_enemy], sizeof(struct Enemy));
}

void set_player_location(Location* loc)
{
	player.location = loc;
	ncurs_location();
}

void ac_dungeons()
{
	set_player_location(&loc_dungeons);
}

void ac_dungeons_action()
{
	if (player.action_points > 0)
	{
		player.action_points--;

		if (check_rnd_events() != 1) {/* return 1 => a random event occurred, don't fight */
			set_player_location(&loc_fight);
			create_enemy();
			werase(gamew);
			/* TODO: UI prettier, do you need fight_youw or not? */
			wprintw(gamew,"You encounter a %s!\n\n", enemy.name);
			mvwprintw(gamew,2,0,"%s",player.name);
			mvwprintw(gamew,2,20,"%s",enemy.name);
			wrefresh(gamew);
	
			fight_youw = newwin(7,10,6,22);
			fight_enemyw = newwin(9,13,6,40);
			ncurs_fightstats(fight_youw);
			ncurs_fightstats_enemy(fight_enemyw);

			/* display skills */
			werase(skillsw);
			for (int i = 0; i < 1; i++)
				wprintw(skillsw,"%d - %s\n",0,weapons[player.weapon_index].name);
			wrefresh(skillsw);

		}
	}
	else
	{
			ncurs_msg("You feel too tired to fight\n");
	}
}

/* helper functions so I don't need to rewrite that much code */
void ac_fight_0() {
ac_fight_fight(0);
}

void ac_fight_1() {
ac_fight_fight(1);
}

void ac_fight_fight(int keypress)
{

/* this function is entered with a keypress from ac_fight0 and so on*/
/* the keypress is used to determine what attack is used */

/* 1. the player already chose the attack (int keypress), now it's time for the enemy */
/* TODO: make the enemy actually attack */
/* TODO: struct enemy needs a weapon etc */

/* 2. calculate damage */
/* player does damage */
/* Wu Xing cycles:
Wood causes +FIRE, -EARTH
Fire causes +EARTH, -METAL
Earth causes +METAL, -WATER
Metal causes +WATER, -WOOD
Water causes +WOOD, -FIRE
*/

switch (weapons[player.weapon_index].dmg_type) {
	case DMG_WOOD:
		enemy.fire++;
		enemy.earth--;
		break;


	case DMG_FIRE:
		enemy.earth++;
		enemy.metal--;
		break;


	case DMG_EARTH:
		enemy.metal++;
		enemy.water--;
		break;


	case DMG_METAL:
		enemy.water++;
		enemy.wood--;
		break;


	case DMG_WATER:
		enemy.wood++;
		enemy.fire--;
		break;

	default:
		break;
	}

/* enemy hits */
/* this is more complicated than player,
for a proof-of-concept battle system:
if skill dmg_type and enemy dominant type match, enemy gets a bonus
*/

switch (enemy.skill->dmg_type) {
	case DMG_WOOD:
		player.fire++;
		player.earth--;
		if (enemy.dom_type == TYPE_WOOD) {
			player.fire++;
			player.earth--;
			}
		break;


	case DMG_FIRE:
		player.earth++;
		player.metal--;
		if (enemy.dom_type == TYPE_FIRE) {
			player.earth++;
			player.metal--;
			}
		break;


	case DMG_EARTH:
		player.metal++;
		player.water--;
		if (enemy.dom_type == TYPE_EARTH) {
			player.metal++;
			player.water--;
			}
		break;


	case DMG_METAL:
		player.water++;
		player.wood--;
		if (enemy.dom_type == TYPE_METAL) {
			player.water++;
			player.wood--;
			}
		break;


	case DMG_WATER:
		player.wood++;
		player.fire--;
		if (enemy.dom_type == TYPE_WATER) {
			player.wood++;
			player.fire--;
			}
		break;

	default:
		break;
	}


/* 3. update stats and display them, TODO: display attack info */

	ncurs_fightstats(fight_youw);
	ncurs_fightstats_enemy(fight_enemyw);

/* 4. check for dead player/enemy */

/* figure out the order of checking deaths: attacks are simultaneous. Iniative? */
/* check if enemy dies */
if (enemy.wood <= 0||enemy.fire <= 0||enemy.earth <= 0||enemy.metal <= 0||enemy.water <= 0)
	{
	mvwprintw(gamew,10,0,"%s is slain!\n", enemy.name);
	int money = 7;
	int exp = 10;
	wprintw(gamew,"You find %d coins on the corpse, and gain %d experience\n", money, exp);
	player.money += money;
	player.experience += exp;
	mvwprintw(gamew,12,0,"<MORE>");
	wrefresh(gamew);
	getch(),
	set_player_location(&loc_dungeons);
	}

/* check if player dies as well */
if (player.wood <= 0||player.fire <= 0||player.earth <= 0||player.metal <= 0||player.water <= 0)
	{
	wclear (gamew);
	mvwprintw(gamew,12,0,"The world fades around you as you fall to the ground,\nbleeding.");
	wattron(gamew,A_BOLD);
	wattron(gamew,A_UNDERLINE);
	mvwprintw(gamew,15,0,"You are dead.");
	wattroff(gamew,A_BOLD);
	wattroff(gamew,A_UNDERLINE);
	playing = false;
	}

wrefresh(gamew);
}

void ac_dungeons_glow()
{
	set_player_location(&loc_shrine);
}

void ac_ev_oldman_help()
{
	wprintw(gamew,"Thank you kind sir! Here take this for your trouble.\n\n");
	wprintw(gamew,"The old man shoves a purse in your hands.\n");
	wattron(gamew,A_BOLD);
	wprintw(gamew,"\n\nYou gain 100 gold!");
	wattroff(gamew,A_BOLD);
	wrefresh(gamew);
	player.money += 100;
	getch();
	set_player_location(&loc_dungeons);
}

void ac_ev_oldman_nohelp()
{
	ncurs_msg("May the gods curse you, mutters the old man.");
	player.action_points++;
	set_player_location(&loc_dungeons);
}

void ac_shrine_heal_1()
{
/*
	if (player.health < player.max_health)
	{
		if (player.money > 0)
		{
			player.health++;
			player.money--;
			puts("The coin disappears into the slot with a clink. The crystal brightens for a moment and you step away from it feeling slightly better.");
		}
		else
		{
			puts("You search your pockets for coin, but find none.");
		}
	}
	else
	{
		puts("You try to insert a coin into the slot but to your surprise the crystal repels your coin.");
	}
*/
}

void ac_shrine_heal_all()
{
/*
	int money = player.money;
	money -= player.max_health - player.health;
	player.health = player.max_health;
	if (money < 0)
	{
		player.health += money;
		money = 0;
	}
	player.money = money;
	player.location = &loc_dungeons;
	puts("The crystal dazes you with blindingly bright light. As you regain control, you notice that all your wounds have mended.");
*/
}

void ac_list_players()
{
	ncurs_msg("\nThe wind is howling in the empty streets of this god forsaken town. You are all alone here.");

}

void ac_view_stats()
{
	ncurs_stats(player);
}

void ac_tavern()
{
	set_player_location(&loc_tavern);
}

void ac_tavern_shout()
{
	puts("What do you want to yell?");
	int len = 80;
	char *line = malloc(len); // more dynamic memory allocation would be nice
	if (getnstr(line, len) != ERR)
	{
		send_chatmsg(line, strlen(line));
	}
	free(line);
}

void ac_shop()
{
	set_player_location(&loc_shop);
}

void ac_shop_buy()
{
	wprintw(gamew,"\nThe poor man is selling these items:\n");
	for (int i=0; i <= WEAPON_NR; i++)
		wprintw(gamew,"%s\n", weapons[i].name);

	wprintw(gamew,"TODO: actually choose what you buy. \nYOU JUST BOUGHT: %s\n",weapons[1].name);
	player.weapon_index = 1;
	wrefresh(gamew);

}

void ac_shop_sell()
{
	ncurs_msg("The poor man has no coins to buy anything from you.");
	wprintw(gamew,"you have: %s\n",weapons[player.weapon_index].name);
	wrefresh(gamew);
}

void ac_messageboard()
{
	set_player_location(&loc_messageboard);
}

void ac_messageboard_view()
{
	wclear(gamew);
	PGresult *res;
	res = PQexecPrepared(conn, "view_messageboard", 0, NULL, NULL, NULL, 0);
	if (PQresultStatus(res) == PGRES_TUPLES_OK)
	{
		int row_count = PQntuples(res);
		int col_count = PQnfields(res);
		for (int i = 0; i < row_count; i++)
		{
			for (int j = 0; j < col_count; j++)
			{
				wprintw(gamew, "%s\t\t", PQgetvalue(res, i, j));
			}
			wprintw(gamew, "\n");
		}
	}
	wrefresh(gamew);
	PQclear(res);
}

void ac_messageboard_write()
{
	int len = 80;
	char *line = malloc(len); // more dynamic memory allocation would be nice
	if (getnstr(line, len) != ERR) // TODO a better way to get input
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
}

void ac_return_to_town()
{
	set_player_location(&loc_town);
}

void ac_quit()
{
	ncurs_msg("Bye.");
	playing = false;
}

