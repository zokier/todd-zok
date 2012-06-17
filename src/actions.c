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

#define DISABLE_COMBAT

// globals are nasty?
Character enemy;

void use_skill(int keypress);
int dmg_calc(int direction);
int dmg_calc_alignbonus(int direction, Element type);
int dmg_calc_blocking(int direction, Element type);
void fight_check_dead();
void align_elements(int direction, Element type);

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
	/* randomly choose an enemy from enemylist, based on player dungeon level */
	int random_enemy = rand() % ENEMY_COUNT;
	memcpy(&enemy,&enemylist[player.dungeon_lvl][random_enemy], sizeof(enemy));
}

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
#ifndef DISABLE_COMBAT
	if (player.action_points > 0)
	{
		player.action_points--;

		if (check_rnd_events() != 1) {/* return 1 => a random event occurred, don't fight */
			set_player_location(&loc_fight);
			create_enemy();
			/* TODO: UI prettier, do you need fight_youw or not? */
			ncurs_log_sysmsg("You encounter a %s!", enemy.name);

			ncurs_fightinfo(player, 0);
			ncurs_fightinfo(enemy, 3);
		}
	}
	else
	{
		ncurs_log_sysmsg("You feel too tired to fight\n");
	}
#else
	ncurs_modal_msg("Tumbleweed rolls across the road. No action here");
#endif
}

/* helper functions so I don't need to rewrite that much code */
void ac_fight_0() {
	use_skill(0);
}

void ac_fight_1() {
	use_skill(1);
}

void use_skill(int keypress)
{

#ifndef DISABLE_COMBAT
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

	dmg_calc(0); /* player hits enemy */
	player.action_points = player.action_points - player.skill->ap_cost; /* spend action points on the attack */

	dmg_calc(1); /* enemy hits player */
	/* TODO: Struct Enemy don't have action points. Once they do, make enemy spend action points as well */

	/* 3. update stats and display them, TODO: display attack info */

	ncurs_fightstats(fight_youw);
	ncurs_fightstats_enemy(fight_enemyw);

	/* 4. check for dead player/enemy */

	fight_check_dead();

	wrefresh(game_win);
#endif
}

void ac_dungeons_glow()
{
	set_player_location(&loc_shrine);
}

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
	int selection = ncurs_listselect(skills_list[0].name, sizeof(Skills), SKILLS_COUNT);
	if (selection > 0 && selection != 'x')
		{
		player.skill[0] = &skills_list[selection];
		ncurs_skills();

		ncurs_log_sysmsg("%s learned a new skill: %s\n",player.name, player.skill[0]->name);
		ncurs_modal_msg("YOU JUST LEARNED: %s",player.skill[0]->name);
		}

	if (selection == 'x') {
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
	int selection = ncurs_listselect(weapons_list[0].name, sizeof(Weapons), WEAPON_COUNT);
	if (selection > 0 && selection != 'x')
		{
			player.weapon = &weapons_list[selection];
			ncurs_log_sysmsg("%s bought %s\n",player.name,player.weapon->name);
			ncurs_modal_msg("YOU JUST BOUGHT: %s",player.weapon->name);
		}

	if (selection == 'x') {
		ncurs_modal_msg("May the gods curse you for wasting my time, mutters the old man.");
		}

	set_player_location(&loc_shop);
}

void ac_shop_sell()
{
	ncurs_modal_msg("The poor man has no coins to buy anything from you.\n"
	                "you have: %s",player.weapon->name);
	set_player_location(&loc_shop);
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
			for (int j = 0; j < col_count; j++)
			{
				wprintw(game_win, "%s\t\t", PQgetvalue(res, i, j));
			}
			wprintw(game_win, "\n");
		}
	}
	wrefresh(game_win);
	PQclear(res);
}

extern char* itoa(int i);
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
	ncurs_modal_msg("Bye.");
	playing = false;
}

#ifndef DISABLE_COMBAT
/* direction: 0 => player hits enemy, 1 => enemy hits player */
/* TODO: is there a simpler way to do this? */

int dmg_calc(int direction) {
	int dmg = 0;

	switch (direction) {
		case 0: { /* player hits enemy */
					/* calculate normal damage done based on skill + weapon */
					dmg = (player.skill->damage + player.weapon->damage);

					/* elements align == player elemental type (dominant element), skill type and weapon type are the same */
					/* if elements align, dmg = dmg + element of player.element_type */
					/* also, if elements align, changes to enemy elemental balance occur */
					if (player.elemental_type == player.skill->dmg_type && player.elemental_type == player.weapon->dmg_type) {
						dmg = dmg + dmg_calc_alignbonus(direction, player.elemental_type);
						align_elements(direction,player.elemental_type);
					}

					/* calculate blocking by enemy*/
					/* blocking elements are based on skill used to attack. TODO: should this be weapon instead? */
					dmg = dmg - dmg_calc_blocking(direction, player.skill->dmg_type);

					/* calculate damage */
					if (dmg > 0) 
						enemy.health = enemy.health - dmg;
					else { /* don't do negative damage */
						wprintw(game_win,"Enemy blocked!\n"); /* TODO: wprintw(logw); */
						wrefresh(game_win);
					}	
					break;
				} /* player hits enemy */

		case 1: { /* enemy hits player */
					/* calculate normal damage done based on skill + weapon */
					dmg = (enemy.skill->damage + enemy.weapon->damage);

					/* elements align == player elemental type (dominant element), skill type and weapon type are the same */
					/* if elements align, dmg = dmg + element of player.element_type */
					/* also, if elements align, changes to enemy elemental balance occur */
					if (enemy.elemental_type == enemy.skill->dmg_type && enemy.elemental_type == enemy.weapon->dmg_type) {
						dmg = dmg + dmg_calc_alignbonus(direction, enemy.elemental_type);
						align_elements(direction,enemy.elemental_type);
					}

					/* calculate blocking by player */
					/* blocking elements are based on skill used to attack. TODO: should this be weapon instead? */
					dmg = dmg - dmg_calc_blocking(direction, enemy.skill->dmg_type);

					/* calculate damage */
					if (dmg > 0) 
						player.health = player.health - dmg;
					else { /* don't do negative damage */
						wprintw(game_win,"You blocked!\n"); /* TODO: wprintw(logw); */
						wrefresh(game_win);
					}
					break;
				}
	}
return 0; /* this should never happen, btw */
}

/* direction: 0 => player->enemy. 1 => enemy->player */
void align_elements(int direction, Element type) {
	switch (direction) {
		case 0: /* player hits enemy, enemy elements change */
			enemy.elements[(type+1) % ELEM_COUNT]++;
			enemy.elements[(type+2) % ELEM_COUNT]--;
			break;
		case 1: /* enemy hits player, player elements change */
			player.elements[(type+1) % ELEM_COUNT]++;
			player.elements[(type+2) % ELEM_COUNT]--;
			break;
	}
}

int dmg_calc_alignbonus(int direction, Element type) {
	switch (direction) {
		case 0: /* 0 = player hits enemy */
			return player.elements[player.elemental_type];
		case 1: /* 1 = enemy hits player */
			return enemy.elements[player.elemental_type];
	}
	return 0;
}

int dmg_calc_blocking(int direction, Element dmg_type)
{
	switch (direction) {
		case 0:
			return enemy.elements[(dmg_type+3) % ELEM_COUNT];
		case 1:
			return player.elements[(dmg_type+3) % ELEM_COUNT];
	}
	return 0;
}


void fight_check_dead() {

	/* TODO: figure out the order of checking deaths: attacks are simultaneous. Iniative? */

	/* check if enemy dies */
	bool enemy_dead = false;
	for (size_t i = 0; i < ELEM_COUNT; i++)
	{
		if(enemy.elements[i] <= 0)
			enemy_dead = true;
	}
	if (enemy.health <= 0)
		enemy_dead = true;
	if (enemy_dead)
	{
		mvwprintw(game_win,14,0,"%s is slain!\n\n", enemy.name);
		int money = 7;
		int exp = 10;
		mvwprintw(game_win,15,0,"You find %d coins on the corpse, and gain %d experience\n", money, exp);
		player.money += money;
		player.experience += exp;
		mvwprintw(game_win,16,0,"<MORE>");
		wrefresh(game_win);
		getch();
		set_player_location(&loc_dungeons);
	}

	/* check if player dies as well */
	bool player_dead = false;
	for (size_t i = 0; i < 5; i++)
	{
		if(player.elements[i] <= 0)
			player_dead = true;
	}
	if (player.health <= 0)
		player_dead = true;
	if (player_dead)
	{
		wclear (game_win);
		mvwprintw(game_win,6,0,"The world fades around you as you fall to the ground,\nbleeding.");
		wattron(game_win,A_BOLD);
		wattron(game_win,A_UNDERLINE);
		mvwprintw(game_win,8,0,"You are dead.");
		wattroff(game_win,A_BOLD);
		wattroff(game_win,A_UNDERLINE);
		playing = false;
	}

}
#endif 
