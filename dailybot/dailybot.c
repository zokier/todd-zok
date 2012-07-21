/*
Daily events bot
AKA Midnight Master
USAGE (add to cron): 
$(BIN) --hourly = hourly events
$(BIN) --daily = daily events
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <syslog.h>
#include <stdbool.h>
#include <time.h>
#include <libpq-fe.h>
#include "globals.h"
#include "database.h"

PGconn *conn;

void hourly();
void daily();
void resetgame();
void stamina();
void random_daily();
void time_dependent();
void quests();
int get_current_time();
int calc_hour(int player_id);

extern int init_pq(); /* from todd.c */

/* time related stuff */
time_t result;
struct tm broken_time;




int main(int argc,char *argv[]) {
openlog("ToDD-DAILYBOT", LOG_PID|LOG_PERROR, LOG_USER);
syslog(LOG_DEBUG,_("Dailybot routine starting.."));

init_pq(); /* found in src/database.c */

if (argc != 1) { /* there's arguments, act on them */
	if (strcmp(argv[1], "--hourly") == 0)
		hourly();
	if (strcmp(argv[1], "--daily") == 0)
		daily();

	if (strcmp(argv[1], "--reset") == 0)
		resetgame();

	}
}


void hourly() {
syslog(LOG_DEBUG,_("Doing an hourly check.."));
stamina();
time_dependent();
}

void daily() {
syslog(LOG_DEBUG,_("Dailybot doing a daily check.."));
random_daily();
time_dependent();
}

void stamina() {
/* TODO: this function should loop through players and increase stamina

For the first 24 hours after login:
if the player is in a room,  increase stamina by 3
in fields, increase by 2

After that:
increase by 2 in the room, 1 in the fields
*/

	/* Loop through all the players and update stamina for them */

	PGresult *res;
	res = PQexecPrepared(conn, "load_player_logout_time_stamina", 0, NULL, NULL, NULL, 0);

	/* TODO: error handling */
	if (PQresultStatus(res) == PGRES_TUPLES_OK)
	{
                int row_count = PQntuples(res);
		syslog(LOG_DEBUG,_("%d players found for stamina update"),row_count);
		for (int i = 0; i < row_count; i++) /* loop through all the players */
		{
		int player_id = atoi(PQgetvalue(res,i,0));
		int player_location = atoi(PQgetvalue(res,i,2));
		int player_stamina = atoi(PQgetvalue(res,i,3));
		int newstamina = 0; /* helper int */

		/* compare last_logout to current time  and update stamina if needed */

		/* Loop conditions */
		/* 1. if stamina >= 100, don't update at all */

		if (player_stamina < STAMINA_MAX) 
 		{
			/* if time since last_logout <= 24 hours */
			int hours_since_logout = calc_hour(player_id);
			
			/* > 0 means the second run of dailybot will update stamina, is it good? */
			/* (>= 0 would be always true) */
			if (hours_since_logout > 0 && hours_since_logout < 24) 
				newstamina = 1;
				
			
	
			/* stamina recovery depends on the place */
			switch (player_location)
			{
				case LOC_ONLINE:
				{
					syslog(LOG_DEBUG,_("Player #%d is online, no recovery!"), player_id);
					break;
				}

				case LOC_DEAD: // TODO: permadeath or no, what to do with dead players?
				case LOC_DEAD_GRAVEYARD:
				{
					syslog(LOG_DEBUG,_("Player #%d is dead, no recovery!"), player_id);
					break;
				}

				case LOC_OFFLINE_ROOM:
				{
					/* within 24 hours from last logout this would be 3, otherwise 1*/
					newstamina += 2; 
					break;
				}

				case LOC_OFFLINE_FIELDS:
				{
					/* within 24 hours from last logout this would be 2, otherwise 1*/
					newstamina++; 
					break;
				}

				case LOC_FAINTED:
				{
					// TODO: rules of fainting..
					break;
				}

				default:
					break;
			}

		/* do the actual update */
		if (player_location == LOC_OFFLINE_ROOM || player_location == LOC_OFFLINE_FIELDS) /* don' update online//dead players */
			{
			int old_stamina = player_stamina;
			player_stamina += newstamina;
			if (player_stamina > STAMINA_MAX) /* don't go over STAMINA_MAX */
				player_stamina = STAMINA_MAX;


 			char *id = itoa(player_id);
			char *stamina = itoa(player_stamina);
 
			const char *params[2] = {id,stamina};
			PGresult *update;
			update = PQexecPrepared(conn, "update_stamina", 2, params, NULL, NULL, 0);
			if (PQresultStatus(update) != PGRES_COMMAND_OK)
			{ /* TODO: since it's a cronjob nobody will ever see this error message..*/
				syslog(LOG_DEBUG,_("Dailybot database update failed!\n"));
			}

		        PQclear(update);
			syslog(LOG_DEBUG,_("Player #%d stamina updated from %d to %d\n"),player_id, old_stamina, player_stamina);
			}
		} /* don't update, since stamina is at max already */
		else
			syslog(LOG_DEBUG,_("Player #%d stamina is already at STAMINA_MAX (%d)\n"),player_id,STAMINA_MAX);
		}
	}
        PQclear(res);
}


void random_daily() {
/* TODO:
random events that happen daily:
	discounts in shop / tavern
	TODO: more random daily events

these all require:
	* new table in database: daily_randoms (or something)

*/
}

void time_dependent() {
/* TODO:
Add date dependent events here.
examples:
	* friday and saturday nights the tavern has a band playing
	* birthdays for Willie & shopkeeper
	* local festival 1st of every month
	* night / day cycle
	* weather
*/
}

int calc_hour(int player_id)
{
	char *id = itoa(player_id);
	const char *params[1] = {id};
	PGresult *res;
	res = PQexecPrepared(conn, "calc_hours", 1, params, NULL, NULL, 0);
	if (PQresultStatus(res) != PGRES_COMMAND_OK)
		{ /* TODO: since it's a cronjob nobody will ever see this error message..*/
		syslog(LOG_DEBUG,_("now() - last_logout failed"));
		}

PQclear(res);
return atoi(PQgetvalue(res,0,0)); /* return now() - last_logout */
}

// resets all databases - use with caution
// basically this is used to reset the game
void resetgame()
{
	syslog(LOG_DEBUG,_("Resetting ToDD..\n"));
	// hopefully this is never ran as a cronjob, so a printf will do just fine..
	printf(_("Resetting ToDD..\n\n\n\n\n"));
	
        PGresult *reset;
        reset = PQexecPrepared(conn, "initdb_dropall", 0, NULL, NULL, NULL, 0);
        if (PQresultStatus(reset) != PGRES_COMMAND_OK)
		{
                syslog(LOG_DEBUG,_("database dropall failed"));
		printf("here: %s\n",PQresultErrorMessage(reset));
		}

        reset = PQexecPrepared(conn, "initdb_createschema", 0, NULL, NULL, NULL, 0);
        if (PQresultStatus(reset) != PGRES_COMMAND_OK)
                syslog(LOG_DEBUG,_("database createschema failed"));

        reset = PQexecPrepared(conn, "initdb_extension_pgcrypto", 0, NULL, NULL, NULL, 0);
        if (PQresultStatus(reset) != PGRES_COMMAND_OK)
                syslog(LOG_DEBUG,_("database pgcrypto failed"));

        reset = PQexecPrepared(conn, "initdb_player_logins", 0, NULL, NULL, NULL, 0);
        if (PQresultStatus(reset) != PGRES_COMMAND_OK)
                syslog(LOG_DEBUG,_("database player_logins failed"));

        reset = PQexecPrepared(conn, "initdb_player_stats", 0, NULL, NULL, NULL, 0);
        if (PQresultStatus(reset) != PGRES_COMMAND_OK)
                syslog(LOG_DEBUG,_("database player_stats failed"));

        reset = PQexecPrepared(conn, "initdb_messageboard", 0, NULL, NULL, NULL, 0);
        if (PQresultStatus(reset) != PGRES_COMMAND_OK)
                syslog(LOG_DEBUG,_("database messageboard failed"));

        reset = PQexecPrepared(conn, "initdb_parties", 0, NULL, NULL, NULL, 0);
        if (PQresultStatus(reset) != PGRES_COMMAND_OK)
                syslog(LOG_DEBUG,_("database parties failed"));

	PQclear(reset);

	printf(_("Finished the reset, hopefully there were no errors..\n"));

}
