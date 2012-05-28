#include <stdio.h>
#include <stdbool.h>
#include "player.h"
#include "locations.h"

extern int playing;

void *create_enemy()
{
	static Enemy enemy;
	enemy = { "Feeble goblin", 10 };
	return &enemy;
}

void set_player_location(void *data, Location* loc)
{
	Player *player = (Player *)data;
	player->location = loc;
}

void ac_dungeons(void *data)
{
	set_player_location(data, &loc_dungeons);
}

void ac_dungeons_action(void *data)
{
	loc_fight.data = create_enemy();
	set_player_location(data, &loc_fight);
}

void ac_dungeons_glow(void *data)
{
	set_player_location(data, &loc_shrine);
}

void ac_shrine_heal_1(void *data)
{
	Player *player = (Player *)data;
	if (player->health < player->max_health)
	{
		if (player->money > 0)
		{
			player->health++;
			money--;
		}
	}
}

void ac_shrine_heal_all(void *data)
{
	Player *player = (Player *)data;
	int money = player->money;
	money -= player->max_health - player->health;
	player->health = player->max_health;
	if (money < 0)
	{
		player->health += money;
		money = 0;
	}
	player->money = money;
	player->location = &loc_dungeons;
}

void ac_list_players(void *data)
{
	puts("The wind is howling in the empty streets of this god forsaken town. You are all alone here.");
}

void ac_view_stats(void *data)
{
	fputs("You are called ", stdout);
	puts(((Player*)data)->name);
	puts("The bards know nothing else about you yet.");
}

void ac_tavern(void *data)
{
	set_player_location(data, &loc_tavern);
}

void ac_shop(void *data)
{
	set_player_location(data, &loc_shop);
}

void ac_shop_buy(void *data)
{
	puts("The poor man has nothing to sell to you.");
}

void ac_shop_sell(void *data)
{
	puts("The poor man has no coins to buy anything with.");
}

void ac_return_to_town(void *data)
{
	set_player_location(data, &loc_town);
}

void ac_quit(void *data)
{
	puts("Bye.");
	playing = false;
}

