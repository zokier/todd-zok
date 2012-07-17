#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <syslog.h>
#include <stdbool.h>
#include <time.h> /* for random number generator */
#include <unistd.h>

#include <zmq.h>
#include <libpq-fe.h>

#include "action.h"
#include "location.h"
#include "locations.h"
#include "networking.h"
#include "ui.h"
#include "weapons.h"
#include "globals.h"
#include "character.h"
#include "database.h"

#define RETRY_LIMIT 3

// ugly globals go here
int playing;
void *push_socket = NULL;
void *chat_socket = NULL;
void *party_socket = NULL;
void *zmq_context = NULL;
bool zmq_python_up();
 
PGconn *conn;


/*
	Asks the player for name
*/
bool get_name()
{
	char *name = NULL;
	size_t name_len = 0;

	// TODO: clear the screen (ncurses?), ascii art and so on
	fputs("\n\n\n\n\nTODO: ASCII ART HERE!\n\n\n\n\n\n",stdout);

	// Introductory text
	fputs(_("Growing up, you heard the local bards sing about ancient dungeons.\n"),stdout);
	fputs(_("The dungeons were filled with unimaginable treasures...and monsters.\n"),stdout);
	fputs(_("\nThat's if you believed the bards. You were now approaching a village next to the dungeons, about to find out.\n"),stdout);

	fputs(_("\nAs you approach the gated village, the Gatekeeper yells at you:\n"),stdout);
	fputs(_("HALT! Who goes there? Annouce yourself: "), stdout);

	// get user input
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

bool get_id()
{
	bool ret = false;
	const char *params[1] = {player.name};
	PGresult *res;
	res = PQexecPrepared(conn, "get_login_id", 1, params, NULL, NULL, 0);
	if (PQresultStatus(res) == PGRES_TUPLES_OK)
	{
		if (PQntuples(res) == 1)
		{
			// TODO get integers directly without atoi
			player.id = atoi(PQgetvalue(res, 0, 0));  
			ret = true;
		}
	}
	PQclear(res);
	return ret;
}

bool check_passwd()
{
	bool ret = false;
	char *passwd = NULL;
	size_t passwd_len = 0;

	// the gatekeeper asks for the password
	printf(_("\nIf you're really %s you'll remember the secret code word we agreed on last time!\n"),player.name);
	printf(_("Tell me the the secret code word and I'll let you pass: "));


	ssize_t line_len = getline(&passwd, &passwd_len, stdin);
	passwd[line_len-1] = '\0'; // strip newline
	char *player_id = itoa(player.id);
	const char *params[2] = {player_id, passwd};
	PGresult *res;
	res = PQexecPrepared(conn, "check_passwd", 2, params, NULL, NULL, 0);
	if (PQresultStatus(res) == PGRES_TUPLES_OK)
	{
		if (PQntuples(res) != 0)
		{
			ret = true;
		}
	}
	PQclear(res);
	free(passwd);
	free(player_id);
	return ret;
}

void load_player_data()
{

	/* first, set the player location to "ONLINE" */
	db_player_location(LOC_ONLINE); 

        PGresult *res;
	char *player_id = itoa(player.id);
	const char *params[1] = {player_id};
	res = PQexecPrepared(conn, "load_player", 1, params, NULL, NULL, 0);
	if (PQresultStatus(res) == PGRES_TUPLES_OK)
	{
		int row_count = PQntuples(res);
		// UNUSED variable: int col_count = PQnfields(res);
		if (row_count > 0)
		{
			if (row_count > 1)
			{
				syslog(LOG_WARNING, "Duplicate player data found. id = %d, name = %s, row count %d", player.id, player.name, row_count);
			}
			// load data from first row even if there is multiple rows
			player.location = &loc_town; // TODO location _should_ be fetched from db!
			int col_cursor = 0;
			player.stamina = atoi(PQgetvalue(res, 0, col_cursor++));
			player.experience = atoi(PQgetvalue(res, 0, col_cursor++));
			player.money = atoi(PQgetvalue(res, 0, col_cursor++));
			player.health = atoi(PQgetvalue(res, 0, col_cursor++));
			player.max_health = atoi(PQgetvalue(res, 0, col_cursor++));
			for (int i = 0; i < ELEM_COUNT; i++)
			{
				player.elements[i] = atoi(PQgetvalue(res, 0, col_cursor++));
			}

			/* id contains the weapon id. Loop through struct weapon to find the corresponding weapon */
			int temp_id = atoi(PQgetvalue(res,0,col_cursor++));
			int i;
			for (i = 0; i <= WEAPON_COUNT; i++)
				if (weapons_list[i].index == temp_id)
					break;

			player.weapon = &weapons_list[i];;

			/* Do the same for skills */
			for (int i = 0; i < 4; i++)
			{
			int temp_skill_id = atoi(PQgetvalue(res,0,col_cursor++));
			int temp_i;
			for (temp_i = 0; temp_i <= SKILLS_COUNT; temp_i++)
				if (skills_list[temp_i].index == temp_skill_id)
					break;

			player.skill[i] = &skills_list[temp_i];
			}


			// reset player dungeon level to 0 (town)
			player.dungeon_lvl = 0;
//			player.dungeon_lvl = atoi(PQgetvalue(res, 0, col_cursor++));
//			if (col_cursor != col_count)
//			{
//				syslog(LOG_WARNING, "col_cursor: %d != col_count: %d", col_cursor, col_count);
//			}
		}
		else
		{
			syslog(LOG_WARNING, "Player data not found. Player id = %d, name = %s", player.id, player.name);
		}
	}
	else
	{
		syslog(LOG_WARNING, "Player data load failed: %s", PQresultErrorMessage(res));
	}
	free(player_id);
	PQclear(res);
}

void save_player_data()
{
	char *player_id = itoa(player.id);
	char *stamina = itoa(player.stamina);
	char *experience = itoa(player.experience);
	char *money = itoa(player.money);
	char *health = itoa(player.health);
	char *max_health = itoa(player.max_health);
	char *elem_0 = itoa(player.elements[0]);
	char *elem_1 = itoa(player.elements[1]);
	char *elem_2 = itoa(player.elements[2]);
	char *elem_3 = itoa(player.elements[3]);
	char *elem_4 = itoa(player.elements[4]);

	// TODO get correct indices

	int weapon_id = player.weapon->index;
	int skills_id[3];
	skills_id[0] = player.skill[0]->index;
	skills_id[1] = player.skill[1]->index;
	skills_id[2] = player.skill[2]->index;
	skills_id[3] = player.skill[3]->index;

	char *weapon = itoa(weapon_id);
	char *skill_0 = itoa(skills_id[0]);
	char *skill_1 = itoa(skills_id[1]);
	char *skill_2 = itoa(skills_id[2]);
	char *skill_3 = itoa(skills_id[3]);

	char *dungeon_lvl = itoa(player.dungeon_lvl); /* TODO: permadeath, this shouldn't be needed */
	const char *params[17] = {
		player_id,
		stamina,
		experience,
		money,
		health,
		max_health,
		elem_0,
		elem_1,
		elem_2,
		elem_3,
		elem_4,
		weapon,
		skill_0,
		skill_1,
		skill_2,
		skill_3,
		dungeon_lvl
	};
	PGresult *res;
	res = PQexecPrepared(conn, "save_player", 17, params, NULL, NULL, 0);
	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		syslog(LOG_WARNING, "Player data save failed: %s", PQresultErrorMessage(res));
	}

	const char *logout[1] = { player_id };
	res = PQexecPrepared(conn, "save_player_logout_time", 1, logout, NULL, NULL, 0);
	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		syslog(LOG_WARNING, "Player data save failed: %s", PQresultErrorMessage(res));
	}
	free(player_id);
	free(stamina);
	free(experience);
	free(money);
	free(health);
	free(max_health);
	free(elem_0);
	free(elem_1);
	free(elem_2);
	free(elem_3);
	free(elem_4);
	free(weapon);
	free(skill_0);
	free(skill_1);
	free(skill_2);
	free(skill_3);
	free(dungeon_lvl);
	PQclear(res);
	wprintw(game_win,_("\nThe bards succesfully recorded your heroic deeds today.\n"));
	wrefresh(game_win);
}

bool check_name()
{
	size_t len = 0;
	char *it = player.name;
	for (char c = *it++; c != '\0'; c = *it++)
	{
		len++;
		if (c < 'a' || c > 'z')
			if (c < 'A' || c > 'Z')
				if (c != ' ' && c != '-')
					return false;
	}
	if (len < NAME_MIN_LENGTH)
		return false;
	if (len > NAME_MAX_LENGTH)
		return false;
	return true;
}

bool create_player()
{
	bool ret = false;
	if (!check_name())
	{
		puts(_("TODO: make this part \"realm-like\""));
		puts(_("Invalid name. Allowed characters: A-Z, a-z, dash and space."));
		printf(_("Minimum name length: %d characters.\n"), NAME_MIN_LENGTH);
		return ret;
	}
	char *passwd = NULL;
	size_t passwd_len = 0;

	// New player joining, let the Gatekeeper to the talking

	printf(_("\nNever heard of you, %s\n"),player.name);
	printf(_("You look like a worthless coward to me. I bet my supper that you are a no good fighter.\n"));
	printf(_("That's right. People from all around the kingdom come here because of the dungeons. "));
	printf(_("Some never leave, you know.\n"));
	printf(_("If you're really serious about meeting an untimely death you should at least talk to Bren first.\n"));
	printf(_("Bren can teach you to take care of yourself. Meet him at his arena in the village.\n"));

	printf(_("\nIn case you run away to your mother and she tells you to come back and die in honor, I want to recognize you.\n"));
	printf(_("Tell me a secret code word so I'll know you the next time you arrive: "));



	// ask for password
	ssize_t line_len = getline(&passwd, &passwd_len, stdin);
	passwd[line_len-1] = '\0'; // strip newline
	// TODO ask password for a second time to avoid typos
	const char *params[2] = {player.name, passwd};
	PGresult *res;
	res = PQexecPrepared(conn, "new_login", 2, params, NULL, NULL, 0);
	if (PQresultStatus(res) == PGRES_TUPLES_OK)
	{
		int row_count = PQntuples(res);
		if (row_count == 1)
		{
			player.id = atoi(PQgetvalue(res, 0, 0));
			PQclear(res);
			char *player_id = itoa(player.id);
			const char *params_stats[1] = {player_id};
			res = PQexecPrepared(conn, "new_player_stats", 1, params_stats, NULL, NULL, 0);
			if (PQresultStatus(res) == PGRES_COMMAND_OK)
			{
				ret = true;
			}
			else
			{
				syslog(LOG_WARNING, "Player data create failed: %s", PQresultErrorMessage(res));
			}
			free(player_id);
		}
		else
		{
			syslog(LOG_WARNING, "Player create failed: no id returned");
		}
	}
	else
	{
		syslog(LOG_WARNING, "Player create failed: %s", PQresultErrorMessage(res));
	}
	PQclear(res);
	free(passwd);
	return ret;
}

/*
	Loads player from disk or if player is not found, creates a new player.
*/
bool get_player()
{
	if (!get_name())
	{
		return false;
	}
	if (get_id())
	{
		for (int retries = 0; retries < RETRY_LIMIT; retries++)
		{
			sleep(1);
			if (check_passwd())
			{
				return true;
			}
			puts(_("Incorrect password."));
		}
		return false;
	}
	else	// get_id failed, that means it's a new player joining!
	{
		return create_player();
	}
}


/*
	Tries to find and call the action user requested
*/
void execute_action(unsigned char cmd_char)
{
	//convert upper case to lower case
	if (cmd_char < 'a' && cmd_char > '9') /* accept letters and numbers */
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
}


/*
	Main gameloop here
*/
void enter_game()
{
	ncurs_location();
	playing = true;
	while (playing)
	{
		unsigned char cmd_char;
		bool getch_res = todd_getchar(&cmd_char);
		if (getch_res == false)
		{
			//eof or other read error
			playing = false;
			break;
		}
		execute_action(cmd_char);
	}

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
	if (!(party_socket = zmq_socket(zmq_context, ZMQ_SUB)))
	{
		return false;
	}
	if (zmq_connect(party_socket, "tcp://localhost:5559"))
	{
		return false;
	}

	if (zmq_setsockopt(chat_socket, ZMQ_SUBSCRIBE, CHATMSG_PREFIX, sizeof(CHATMSG_PREFIX)-1)) // strip null terminator
	{
		return false;
	}

	if (zmq_setsockopt(chat_socket, ZMQ_SUBSCRIBE, CTRLMSG_PREFIX, sizeof(CTRLMSG_PREFIX)-1)) // strip null terminator
	{
		return false;
	}

	if (zmq_setsockopt(chat_socket, ZMQ_SUBSCRIBE, DEBUGMSG_PREFIX, sizeof(DEBUGMSG_PREFIX)-1)) // strip null terminator 
	{
		return false;
	}

	int linger_time = 250; // pending messages linger for 250 ms if socket is closed
	if (zmq_setsockopt(push_socket, ZMQ_LINGER, &linger_time, sizeof(linger_time)))
		syslog(LOG_WARNING, "Can not set ZMQ_LINGER: %s", zmq_strerror(errno));

	return true;
}

void cleanup_zmq()
{
	zmq_close(chat_socket);
	zmq_close(push_socket);
	zmq_close(party_socket);
	zmq_term(zmq_context);
}

bool unsub_party(unsigned int id)
{
	size_t len = 0;
	char buf[12]; // 'pXXXXXXXXXX\0' = 12 chars
	len = snprintf(buf, 12, "p%010d", id);
	if (zmq_setsockopt(party_socket, ZMQ_UNSUBSCRIBE, buf, len))
	{
		syslog(LOG_WARNING, "Party unsubscription failed: %s", zmq_strerror(errno));
		return false;
	}
	return true;
}

bool sub_party(unsigned int id)
{
	size_t len = 0;
	char buf[12]; // 'pXXXXXXXXXX\0' = 12 chars
	len = snprintf(buf, 12, "p%010d", id);
	if (zmq_setsockopt(party_socket, ZMQ_SUBSCRIBE, buf, len))
	{
		syslog(LOG_WARNING, "Party subscription failed: %s", zmq_strerror(errno));
		return false;
	}
	return true;
}

/*
 * Unsubscribe from current party in party socket and subscribe to new party id
 */
void set_party(unsigned int id)
{
	if (!unsub_party(player_party.id))
	{
		return;
	}
	if (!sub_party(id))
	{
		return;
	}
	player_party.id = id;
}

/*
	Main entry point. Initializes global resources, logs player on 
	and enters game.
*/
int main(int argc, char *argv[])
{
	int return_code = EXIT_FAILURE;
	openlog("ToDD", LOG_PID|LOG_PERROR, LOG_USER);
	srand((unsigned int)time(NULL));

	if (!init_pq())
	{
		goto cleanup;
	}

	if (!init_zmq())
	{
		syslog(LOG_ERR, "ZeroMQ init failure: %s", zmq_strerror(errno));
		goto cleanup;
	}

	if (!zmq_python_up())
	{
		syslog(LOG_ERR, "Python chat server not responding!");
		syslog(LOG_ERR, "Ensure that server is running and try again!");
		goto cleanup;
	}

	if (!get_player())
	{
		syslog(LOG_ERR, "Player auth failure");
		goto cleanup;
	}

	load_player_data();

	// Don't subscribe to party at startup
//	player_party.id = rand();
//	sub_party(player_party.id);

	init_ui();
	enter_game();
	save_player_data();

	return_code = EXIT_SUCCESS; // returned from game, success

cleanup:
	cleanup_zmq();
	cleanup_pq();
	player.name = NULL; /* without this, free(player.name) segfaults. TODO: figure this one out */
	free(player.name);
	closelog();
	endwin(); /* for curses */
	printf("\n"); /* cleaner exit: user console is undistorted by ncurses stuff */
	return return_code;
}

/* Function checks if python chat server is running. It must be! */
bool zmq_python_up()
{
	/* send a magic line, if you don't receive it, python server doesn't work correctly */
	#define MAGIC "ToDD-MAGIC321"

	char *msg = NULL;
	int token = rand();
	// TODO figure out correct length
	char msg_out[40];
	size_t len = snprintf(&msg_out[0], 40, "%s:%x", MAGIC, token)+1;
	zmq_pollitem_t items [2];
	items[0].socket = chat_socket;
	items[0].events = ZMQ_POLLIN;

		Message msg_foo = create_ctrl_msg(msg_out, len);
	// retry three times, sometimes zmq is slow to start
	for (int i = 0; i < 3; i++)
	{
		send_msg(msg_foo);
		int rc = zmq_poll (items, 1, 1000000);
		if (rc < 0)
		{
			syslog(LOG_WARNING, "ZMQ poll failure: %s", zmq_strerror(errno));
			continue;
		}
		else if (rc != 1)
		{
			syslog(LOG_WARNING, "ZMQ poll failure: no events");
			continue;
		}
		if (items[0].revents & ZMQ_POLLIN)
			msg = try_recv_msg(chat_socket);

		if (msg == NULL)
		{
			syslog(LOG_WARNING, "ZMQ recv failure: %s", zmq_strerror(errno));
			continue;
		}
		// TODO check the length of the msg
		// maybe strtok should be used instead of +sizeof(DEBUGMSG_PREFIX)
		if (strcmp(msg_out,msg+sizeof(CTRLMSG_PREFIX)) != 0) /* TODO???*/
		{
			syslog(LOG_WARNING, "ERROR received %s", msg);
			del_msg(msg_foo);
			continue;
		}
		del_msg(msg_foo);
		return true;
	}
	syslog(LOG_WARNING, "ZMQ chat test retry count exceeded");
	return false;
}
