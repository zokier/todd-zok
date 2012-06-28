#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <syslog.h>
#include <stdbool.h>
#include <time.h> /* for random number generator */
#include <unistd.h>

#include <zmq.h>
#include <libpq-fe.h>

#include "globals.h"
#include "action.h"
#include "location.h"
#include "locations.h"
#include "networking.h"
#include "ui.h"
#include "weapons.h"
#include "database.h"
#include "character.h"

int init_pq()
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
	res = PQprepare(conn, "new_login", "insert into player_logins (name, passwd) values ($1, crypt(cast($2 as text), gen_salt('bf'))) returning player_logins.id;", 2, NULL);
	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		goto pq_cleanup;
	}
	PQclear(res);
	res = PQprepare(conn, "save_player", "update player_stats set (stamina, experience, money, health, max_health, wood, fire, earth, metal, water, weapon, skill_0, skill_1, skill_2, skill_3, dungeon_level) = ($2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17) where id = $1;", 17, NULL);
	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		goto pq_cleanup;
	}

	res = PQprepare(conn, "player_onlinestatus", "update player_stats set (location) = ($2) where id = $1;", 2, NULL);
	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		goto pq_cleanup;
	}

	res = PQprepare(conn, "save_player_logout_time", "update player_logins set (last_logout) = (now()) where id = $1;", 2, NULL);
	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		goto pq_cleanup;
	}

	
	/* select id, last_logout time and stamina */
	res = PQprepare(conn, "load_player_logout_time_stamina", "select player_logins.id, player_logins.last_logout, player_stats.location, player_stats.stamina from player_logins, player_stats WHERE player_logins.id = player_stats.id;", 0, NULL);
	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		goto pq_cleanup;
	}

	PQclear(res);
	res = PQprepare(conn, "update_stamina", "update player_stats set (stamina) = ($2) where id = $1;", 2, NULL);
	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		goto pq_cleanup;
	}

	PQclear(res);
	res = PQprepare(conn, "new_player_stats", "insert into player_stats (id) values ($1);", 1, NULL);
	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		goto pq_cleanup;
	}
	PQclear(res);
	res = PQprepare(conn, "load_player", "select stamina, experience, money, health, max_health, wood, fire, earth, metal, water, weapon, skill_0, skill_1, skill_2, skill_3, dungeon_level from player_stats where id = $1;", 1, NULL);
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

char *itoa(int i)
{
        char *str = malloc(20); // should be enough?
        snprintf(str, 20, "%d", i);
        return str;
}

/* sets the player location in the database. Knowledge is used by dailybot, at the moment ToDD has no use for this info */
void db_player_location(int location) 
{
	PGresult *res;
	char *player_id = itoa(player.id);
	char *player_location = itoa(location);

	const char *locparams[2] = {player_id, player_location };
	res = PQexecPrepared(conn, "player_onlinestatus", 2, locparams, NULL, NULL, 0);
	if (PQresultStatus(res) != PGRES_COMMAND_OK)
		{
		syslog(LOG_WARNING, "Player online status save failed: %s", PQresultErrorMessage(res));
		}

}
