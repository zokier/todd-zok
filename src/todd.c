#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <syslog.h>
#include <stdbool.h>

#include <zmq.h>

#include "action.h"
#include "location.h"
#include "player.h"
#include "locations.h"
#include "networking.h"

#define NAME_QUERY "What's your name, adventurer?  "
#define WELCOME_NEW "The bards have not heard of you before.\nWelcome to Tales of Deep Dungeons, "
#define WELCOME_OLD "Welcome back to Tales of Deep Dungeons, "


// ugly globals go here
int playing;
void *push_socket = NULL;
void *chat_socket = NULL;
void *zmq_context = NULL;
Player player;

/*
	Creates a new player
*/
int create_player()
{
	player.location = &loc_town;
	player.action_points = 10;
	player.experience = 0;
	player.max_health = 20;
	player.health = player.max_health;
	player.money = 10;
	fputs(WELCOME_NEW, stdout);
	puts(player.name);
	return true;
}

/*
	Loads player from disk
*/
int load_player()
{
	// TODO implementation
	return false;
	fputs(WELCOME_OLD, stdout);
	puts(player.name);
}

/*
	Loads player from disk or if player is not found, creates a new player.
*/
int get_player()
{
	if (load_player())
	{
		// Old player found, returning
		return true;
	}
	else
	{
		return create_player();
	}
}

/*
	Prints descriptions at current player position
*/
void print_location_info()
{
	puts(player.location->description);
	for (size_t i = 0; i < player.location->action_count; i++)
	{
		printf("\t* %s\n", player.location->actions[i].description);
	}
}

/*
	Tries to find and call the action user requested
*/
void execute_action(char cmd_char)
{
	//convert upper case to lower case
	if (cmd_char < 'a')
	{
		cmd_char += 'a' - 'A';
	}
	// search for the corresponding action
	for (size_t i = 0; i < player.location->action_count; i++)
	{
		if (player.location->actions[i].hotkey == cmd_char)
		{
			// execute action
			player.location->actions[i].function();
			break;
		}
	}

	if (playing)
	{
		print_location_info();
	}
}

/*
	Main gameloop here
*/
void enter_game()
{
	print_location_info();
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
		execute_action(line[0]);
	}
	free(line);
}

/*
	Initializes ZeroMQ context and sockets
*/
bool init_zmq()
{
	if (!(zmq_context = zmq_init(1)))
	{
		return false;
	}
	if (!(push_socket = zmq_socket(zmq_context, ZMQ_PUSH)))
	{
		return false;
	}
	if (zmq_connect(push_socket, "tcp://localhost:5558"))
	{
		return false;
	}
	if (!(chat_socket = zmq_socket(zmq_context, ZMQ_SUB)))
	{
		return false;
	}
	if (zmq_connect(chat_socket, "tcp://localhost:5559"))
	{
		return false;
	}
	if (zmq_setsockopt(chat_socket, ZMQ_SUBSCRIBE,
			CHATMSG_PREFIX, sizeof(CHATMSG_PREFIX)-2)) // strip trailing space
	{
		return false;
	}
	int linger_time = 250; // pending messages linger for 250 ms if socket is closed
	if (zmq_setsockopt(chat_socket, ZMQ_LINGER,
			&linger_time, sizeof(linger_time))) // strip trailing space
	{
		syslog(LOG_WARNING, "Can not set ZMQ_LINGER: %s", zmq_strerror(errno));
	}
	return true;
}

/*
	Asks the player for name
*/
bool get_name()
{
	char *name = NULL;
	size_t name_len = 0;
	fputs(NAME_QUERY, stdout);
	ssize_t line_len = getline(&name, &name_len, stdin);
	if (line_len < 0)
	{
		syslog(LOG_ERR, "Read error: %s (%s:%d)", strerror(errno), __FILE__, __LINE__);
		return false;
	}
	name_len = line_len;
	name[name_len - 1] = 0; // strip trailing newline
	if (strnlen(name, name_len) < name_len - 1)
	{
		// null bytes in name
		syslog(LOG_ERR, "Player name contains \\0");
		return false;
	}

	player.name = name;
	return true;
}

/*
	Main entry point. Initializes global resources, logs player on 
	and enters game.
*/
int main(int argc, char *argv[])
{
	int return_code = EXIT_FAILURE;
	openlog("ToDD", LOG_PID|LOG_PERROR, LOG_USER);

	if (!init_zmq())
	{
		syslog(LOG_ERR, "ZeroMQ init failure: %s", zmq_strerror(errno));
		goto cleanup;
	}

	if (!get_name())
	{
		goto cleanup;
	}
	get_player();

	enter_game();

	return_code = EXIT_SUCCESS; // returned from game, success

cleanup:
	zmq_close(chat_socket);
	zmq_close(push_socket);
	zmq_term(zmq_context);
	free(player.name);
	closelog();
	return return_code;
}
