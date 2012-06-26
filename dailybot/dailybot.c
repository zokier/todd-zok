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
#include <libpq-fe.h>
#include "globals.h"

PGconn *conn;

void hourly();
void daily();
void stamina();
void random_daily();
void time_dependent();
void quests();
extern int init_pq(); /* from todd.c */

int main(int argc,char *argv[]) {
init_pq(); /* found in src/database.c */

if (argc != 1) { /* there's arguments, act on them */
	if (strcmp(argv[1], "--hourly") == 0)
		hourly();
	if (strcmp(argv[1], "--daily") == 0)
		daily();
	}

}


void hourly() {
stamina();
time_dependent();
}

void daily() {
random_daily();
time_dependent();
}

void stamina() {
/* TODO: this function should loop through players and increase stamina
requires: 
	* player location in database (In a room  versus ..somewhere else)
	* player last login time in database

For the first 24 hours after login:
if the player is in a room (and thus offline), increase stamina by 3
otherwise, increase by 2

After that:
if the player is in a room (and thus offline), increase stamina by 2
otherwise, increase by 1

NOTE: player stamina maximum is not decided, I think. let it be 100?
	* configuration variable MAX_STAMINA */

	/* Loop through all the players and update stamina for them */
	/* TODO_FINDOUTMAXPLAYERS should contain number of players in table player_logins */
	int TODO_FINDOUTMAXPLAYERS = 50;
	for (int i = 0; i < TODO_FINDOUTMAXPLAYERS; i++) {
	int stamina = 0;
	        char *player_id = itoa(i);
	        const char *params[1] = {player_id};
	        PGresult *res;
	        res = PQexecPrepared(conn, "load_player", 1, params, NULL, NULL, 0);
	        if (PQresultStatus(res) == PGRES_TUPLES_OK)
	        {
 	               int row_count = PQntuples(res);
	                if (row_count > 0)
	                {
        	                if (row_count > 1)
                	        {
                        	        syslog(LOG_WARNING, "Duplicate player data found!\nTODO: bug message\n");
	                        }

        	                // load data from first row even if there is multiple rows
	                        int col_cursor = 0;
                	        stamina = atoi(PQgetvalue(res, 0, col_cursor++));

				/* Code for updating stamina is here */
				/* TODO: last_login should contain last login time */
				int last_login = 0;
				/* TODO: current_time should contain current time */
				int current_time = 0;
				
				/* TODO: actual update code here */

			}
 		}
	}
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
