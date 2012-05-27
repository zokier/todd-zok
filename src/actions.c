#include <stdio.h>
#include <stdbool.h>
#include "player.h"
#include "locations.h"

extern int playing;

void set_player_location(void *data, Location* loc)
{
	Player *player = (Player *)data;
	player->location = loc;
}

void ac_dungeons(void *data)
{
	set_player_location(data, &loc_dungeons);
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

