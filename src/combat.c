#include <stdlib.h>
#include <string.h>
#include "element.h"
#include "character.h"
#include "skills.h"
#include "globals.h"
#include "ui.h"
#include "location.h"
#include "locations.h"

Character enemy;

extern void set_player_location(Location* loc);

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
		int money = 7;
		int exp = 10;
		player.money += money;
		player.experience += exp;
		ncurs_modal_msg(
			"%s is slain!\n\n"
			"You find %d coins on the corpse, and gain %d experience\n", 
			enemy.name, money, exp);
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
		getch();
		playing = false;
	}

}

void create_enemy()
{
	/* randomly choose an enemy from enemylist, based on player dungeon level */
	int random_enemy = rand() % ENEMY_COUNT;
	memcpy(&enemy,&enemylist[player.dungeon_lvl][random_enemy], sizeof(enemy));
}


void align_elements(Character *dest, Element type) {
	dest->elements[(type+1) % ELEM_COUNT]++;
	dest->elements[(type+2) % ELEM_COUNT]--;
}

int dmg_calc_blocking(Character *dest, Element dmg_type)
{
	return dest->elements[(dmg_type+3) % ELEM_COUNT];
}


void skill_effect(Character *source, Character *dest, Skills *skill)
{
	int dmg = 0;
	/* calculate normal damage done based on skill + weapon */
	dmg += skill->damage;
	dmg += source->weapon->damage;

	/* elements align == player skill type and weapon type are the same */
	/* also, if elements align, changes to enemy elemental balance occur */
	if (skill->dmg_type == source->weapon->dmg_type) {
		dmg += source->elements[skill->dmg_type];
		align_elements(dest, skill->dmg_type); // TODO is this correct?
	}

	/* calculate blocking by enemy*/
	/* blocking elements are based on skill used to attack. TODO: should this be weapon instead? */
	dmg -= dmg_calc_blocking(dest, skill->dmg_type);

	/* calculate damage */
	if (dmg > 0)
	{
		dest->health -= dmg;
		ncurs_log_sysmsg("%s did %d damage to %s", source->name, dmg, dest->name);
	}
	else
	{ /* don't do negative damage */
		ncurs_log_sysmsg("%s blcked attack from %s.", dest->name, source->name);
	}
	source->action_points -= skill->ap_cost; /* spend action points on the attack */
}

void use_skill(int keypress)
{
	/* this function is entered with a keypress from ac_fight0 and so on*/
	/* the keypress is used to determine what attack is used */

	/* 1. the player already chose the attack (int keypress), now it's time for the enemy */
	// TODO enemy always uses skill 0

	/* 2. calculate damage */
	/* player does damage */
	/* Wu Xing cycles:
	   Wood causes +FIRE, -EARTH
	   Fire causes +EARTH, -METAL
	   Earth causes +METAL, -WATER
	   Metal causes +WATER, -WOOD
	   Water causes +WOOD, -FIRE
	   */

	if (keypress < 4 && player.skill[keypress] != NULL)
	{
		skill_effect(&player, &enemy, player.skill[keypress]);
		// Enemy attack
		skill_effect(&enemy, &player, enemy.skill[0]);

		/* 3. update stats and display them */
		ncurs_fightinfo(&player, 0);
		ncurs_fightinfo(&enemy, 3);

		/* 4. check for dead player/enemy */
		fight_check_dead();
		wrefresh(game_win);
	}
	else
	{
		ncurs_log_sysmsg("You dont have a skill in slot %d", keypress+1);
	}
}
