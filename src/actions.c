#include <stdio.h>
#include <stdbool.h>
#include "player.h"
#include "locations.h"
#include "enemy.h"
#include "networking.h"

extern int playing;

// globals are nasty?
Enemy enemy;

void create_enemy()
{
	//enemy = { "Feeble goblin", 10 };
	//enemy = { "Fierce goblin", 20 };
	enemy.name = "Feeble goblin";
	enemy.health = 10;
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
	Player *player = (Player *)data;
	if (player->action_points > 0)
	{
		create_enemy();
		printf("You encounter %s!", enemy.name);
		set_player_location(data, &loc_fight);
	}
	else
	{
		puts("You feel too tired to do anything.");
	}
}

void ac_fight_fight(void *data)
{
	Player *player = (Player *)data;
	int damage = 3; // chosen by fair dice roll
	printf("You hit %s with %d points of damage\n", enemy.name, damage);
	enemy.health -= damage;
	if (enemy.health <= 0)
	{
		puts("The enemy is slain!");
		int money = 7;
		int exp = 10;
		printf("You find %d coins on the corpse, and gain %d experience\n", money, exp);
		player->money += money;
		player->experience += exp;
		set_player_location(data, &loc_dungeons);
	}
	else
	{
		damage = 2; // goblins are bit weaker than you
		printf("%s hits you with %d points of damage\n", enemy.name, damage);
		player->health -= damage;
		if (player->health <= 0)
		{
			puts("The world fades around you as you fall to the ground bleeding");
			puts("You are dead.");
			playing = false;
		}
	}
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
			player->money--;
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
	puts("The crystal dazes you with blindingly bright light. As you regain control, you notice that all your wounds have mended.");
}

void ac_list_players(void *data)
{
	puts("The wind is howling in the empty streets of this god forsaken town. You are all alone here.");
}

void ac_view_stats(void *data)
{
	// TODO prettify
	Player *player = (Player *)data;
	printf("name:\t%s\n", player->name);
	printf("action_points:\t%d\n", player->action_points);
	printf("experience:\t%d\n", player->experience);
	printf("max_health:\t%d\n", player->max_health);
	printf("health:\t%d\n", player->health);
	printf("money:\t%d\n", player->money);
}

void ac_tavern(void *data)
{
	set_player_location(data, &loc_tavern);
}

void ac_tavern_shout(void *data)
{
	Player *player = (Player *)data;
	puts("What do you want to yell?");
	char *line = NULL;
	size_t line_len = 0;
	ssize_t len = getline(&line, &line_len, stdin);
	send_chatmsg(player->name, line);
}

void ac_tavern_sit(void *data)
{
	puts("You sit down for a minute to relax.");
	char *msg = try_recv_chatmsg();
	if (msg == NULL)
	{
		puts("That was nice and quiet.");
	}
	else
	{
		puts("Suddenly you hear a voice appear from thin air, saying:");
		puts(msg);
	}
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

