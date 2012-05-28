#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <syslog.h>
#include <stdbool.h>

#include "action.h"
#include "location.h"
#include "player.h"
#include "locations.h"

#define NAME_QUERY "What's your name, adventurer?  "
#define WELCOME_NEW "The bards have not heard of you before.\r\nWelcome to Tales of Deep Dungeons, "
#define WELCOME_OLD "Welcome back to Tales of Deep Dungeons, "


int playing;

/*
	Creates a new player
*/
int create_player(Player *dst)
{
	dst->location = &loc_town;
	dst->action_points = 10;
	dst->experience = 0;
	dst->max_health = 20;
	dst->health = dst->max_health;
	dst->money = 10;
	fputs(WELCOME_NEW, stdout);
	puts(dst->name);
	return true;
}

/*
	Loads player from disk
*/
int load_player(Player *dst)
{
	// TODO implementation
	return false;
	fputs(WELCOME_OLD, stdout);
	puts(dst->name);
}

/*
	Loads player from disk or if player is not found, creates a new player.
*/
int get_player(Player *dst)
{
	if (load_player(dst))
	{
		// Old player found, returning
		return true;
	}
	else
	{
		return create_player(dst);
	}
}

/*
	Prints descriptions at current player position
*/
void print_location_info(Player *player)
{
	puts(player->location->description);
	for (size_t i = 0; i < player->location->action_count; i++)
	{
		printf("\t* %s\n", player->location->actions[i].description);
	}
}

/*
	Tries to find and call the action user requested
*/
void execute_action(Player *player, char cmd_char)
{
	//convert upper case to lower case
	if (cmd_char < 'a')
	{
		cmd_char += 'a' - 'A';
	}
	// search for the corresponding action
	for (size_t i = 0; i < player->location->action_count; i++)
	{
		if (player->location->actions[i].hotkey == cmd_char)
		{
			// execute action
			player->location->actions[i].function(player);
			break;
		}
	}

	if (playing)
	{
		print_location_info(player);
	}
}

/*
	Main gameloop here
*/
void enter_game(Player *player)
{
	print_location_info(player);
	char *line = NULL;
	size_t line_len = 0;
	playing = true;
	while (playing)
	{
		ssize_t len = getline(&line, &line_len, stdin);
		if (len < 0)
		{
			//eof or other read error
			playing = false;
			break;
		}
		execute_action(player, line[0]);
	}
	free(line);
}

/*
	Main entry point. Initializes global resources, logs player on 
	and enters game.
*/
int main(int argc, char *argv[])
{
	int return_code = EXIT_SUCCESS;
	openlog("ToDD", LOG_PID, LOG_USER);

	char *name = NULL;
	size_t name_len = 0;
	fputs(NAME_QUERY, stdout);
	ssize_t line_len = getline(&name, &name_len, stdin);
	if (line_len < 0)
	{
		syslog(LOG_ERR, "Read error %s:%d", __FILE__, __LINE__);
		return_code = EXIT_FAILURE;
		goto cleanup;
	}
	name_len = line_len;
	name[name_len - 1] = 0; // strip trailing newline
	if (strnlen(name, name_len) < name_len - 1)
	{
		// null bytes in name
		syslog(LOG_ERR, "Player name contains \\0");
		return_code = EXIT_FAILURE;
		goto cleanup;
	}

	Player player;
	player.name = name;
	get_player(&player);

	enter_game(&player);

cleanup:
	closelog();
	free(name);
	return return_code;
}
