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

#define NAME_QUERY "What's your name, adventurer?  "
#define WELCOME_NEW "The bards have not heard of you before.\nWelcome to Tales of Deep Dungeons, "
#define WELCOME_OLD "Welcome back to Tales of Deep Dungeons, "
#define RETRY_LIMIT 3
#define NAME_MIN_LENGTH 4
#define NAME_MAX_LENGTH 16

// ugly globals go here
int playing;
void *push_socket = NULL;
void *chat_socket = NULL;
void *zmq_context = NULL;
Character player;
PGconn *conn;

char *itoa(int i)
{
	char *str = malloc(20); // should be enough?
	snprintf(str, 20, "%d", i);
	return str;
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
	printf("Welcome number %d, enter password: ", player.id);
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
	char *player_id = itoa(player.id);
	const char *params[1] = {player_id};
	PGresult *res;
	res = PQexecPrepared(conn, "load_player", 1, params, NULL, NULL, 0);
	if (PQresultStatus(res) == PGRES_TUPLES_OK)
	{
		int row_count = PQntuples(res);
		int col_count = PQnfields(res);
		if (row_count > 0)
		{
			if (row_count > 1)
			{
				syslog(LOG_WARNING, "Duplicate player data found. id = %d, name = %s, row count %d", player.id, player.name, row_count);
			}
			// load data from first row even if there is multiple rows
			player.location = &loc_town; // TODO should location be fetched from db??
			int col_cursor = 0;
			player.action_points = atoi(PQgetvalue(res, 0, col_cursor++));
			player.experience = atoi(PQgetvalue(res, 0, col_cursor++));
			player.money = atoi(PQgetvalue(res, 0, col_cursor++));
			player.health = atoi(PQgetvalue(res, 0, col_cursor++));
			player.max_health = atoi(PQgetvalue(res, 0, col_cursor++));
			for (int i = 0; i < ELEM_COUNT; i++)
			{
				player.elements[i] = atoi(PQgetvalue(res, 0, col_cursor++));
			}
			player.weapon = &weapons_list[atoi(PQgetvalue(res, 0, col_cursor++))];
			for (int i = 0; i < 4; i++)
			{
				player.skill[i] = &skills_list[atoi(PQgetvalue(res, 0, col_cursor++))];
			}
			player.dungeon_lvl = atoi(PQgetvalue(res, 0, col_cursor++));
			if (col_cursor != col_count)
			{
				syslog(LOG_WARNING, "col_cursor: %d != col_count: %d", col_cursor, col_count);
			}
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
	char *action_points = itoa(player.action_points);
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
	char *weapon = "0";
	char *skill_0 = "0";
	char *skill_1 = "0";
	char *skill_2 = "0";
	char *skill_3 = "0";
	char *dungeon_lvl = itoa(player.dungeon_lvl);
	const char *params[17] = {
		player_id,
		action_points,
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
	free(player_id);
	free(action_points);
	free(experience);
	free(money);
	free(health);
	free(max_health);
	free(elem_0);
	free(elem_1);
	free(elem_2);
	free(elem_3);
	free(elem_4);
//	free(weapon);
//	free(skill_0);
//	free(skill_1);
//	free(skill_2);
//	free(skill_3);
	free(dungeon_lvl);
	PQclear(res);
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
		puts("Invalid name. Allowed characters: A-Z, a-z, dash and space.");
		printf("Minimum name length: %d characters.\n", NAME_MIN_LENGTH);
		return ret;
	}
	char *passwd = NULL;
	size_t passwd_len = 0;
	printf("Welcome new player, enter password: ");
	ssize_t line_len = getline(&passwd, &passwd_len, stdin);
	passwd[line_len-1] = '\0'; // strip newline
	// TODO ask password for a second time to avoid typos
	const char *params[2] = {player.name, passwd};
	PGresult *res;
	res = PQexecPrepared(conn, "new_login", 2, params, NULL, NULL, 0);
	if (PQresultStatus(res) == PGRES_COMMAND_OK)
	{
		ret = true;
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
			puts("Incorrect password.");
		}
		return false;
	}
	else
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
		int getch_res = getch();
		if (getch_res == ERR)
		{
			//eof or other read error
			playing = false;
			break;
		}
		if (getch_res == 9) { /* chat window */
			ncurs_chat(player);
		}
		else
			execute_action(getch_res & 0xFF);
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

void cleanup_zmq()
{
	zmq_close(chat_socket);
	zmq_close(push_socket);
	zmq_term(zmq_context);
}

bool init_pq()
{
	PGresult *res = NULL;
    conn = PQconnectdb("dbname=todd user=todd");
    if (PQstatus(conn) != CONNECTION_OK)
	{
		goto pq_cleanup;
	}
	res = PQprepare(conn, "get_login_id", "select id from player_logins where name = $1;", 1, NULL);
	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		goto pq_cleanup;
	}
	PQclear(res);
	res = PQprepare(conn, "check_passwd", "select true from player_logins where id = $1 and passwd = crypt(cast($2 as text), passwd);", 2, NULL);
	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		goto pq_cleanup;
	}
	PQclear(res);
	res = PQprepare(conn, "new_login", "insert into player_logins (name, passwd) values ($1, crypt(cast($2 as text), gen_salt('bf')));", 2, NULL);
	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		goto pq_cleanup;
	}
	PQclear(res);
	res = PQprepare(conn, "save_player", "update player_stats set (action_points, experience, money, health, max_health, wood, fire, earth, metal, water, weapon, skill_0, skill_1, skill_2, skill_3, dungeon_level) = ($2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17) where id = $1;", 17, NULL);
	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		goto pq_cleanup;
	}
	PQclear(res);
	res = PQprepare(conn, "load_player", "select action_points, experience, money, health, max_health, wood, fire, earth, metal, water, weapon, skill_0, skill_1, skill_2, skill_3, dungeon_level from player_stats where id = $1;", 1, NULL);
	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		goto pq_cleanup;
	}
	PQclear(res);
	res = PQprepare(conn, "view_messageboard", "select name, timestamp, body from messageboard, player_logins where player_logins.id = messageboard.player_id order by messageboard.id desc limit 10;", 0, NULL);
	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		goto pq_cleanup;
	}
	PQclear(res);
	res = PQprepare(conn, "write_to_messageboard", "insert into messageboard (player_id, body) values ($1, $2);", 2, NULL);
	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		goto pq_cleanup;
	}
	PQclear(res);

	return true;

pq_cleanup:
	syslog(LOG_ERR, "Postgres init error: %s", PQerrorMessage(conn));
	if (res != NULL)
		PQclear(res);
	return false;
}

void cleanup_pq()
{
    PQfinish(conn);
}

/*
	Main entry point. Initializes global resources, logs player on 
	and enters game.
*/
int main(int argc, char *argv[])
{
	int return_code = EXIT_FAILURE;
	openlog("ToDD", LOG_PID|LOG_PERROR, LOG_USER);

	if (!init_pq())
	{
		goto cleanup;
	}

	if (!init_zmq())
	{
		syslog(LOG_ERR, "ZeroMQ init failure: %s", zmq_strerror(errno));
		goto cleanup;
	}
	srand((unsigned int)time(NULL));

	if (!get_player())
	{
		syslog(LOG_ERR, "Player auth failure");
		goto cleanup;
	}
	load_player_data();

	init_ui();
	enter_game();
	save_player_data();

	return_code = EXIT_SUCCESS; // returned from game, success

cleanup:
	cleanup_zmq();
	cleanup_pq();
	free(player.name);
	closelog();
	endwin(); /* for curses */
	printf("\n"); /* cleaner exit: user console is undistorted by ncurses stuff */
	return return_code;
}
