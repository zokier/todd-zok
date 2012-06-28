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
void stamina();
void random_daily();
void time_dependent();
void quests();
int get_current_time();
int calc_hours(char *asciidate);

extern int init_pq(); /* from todd.c */

/* time related stuff */
time_t result;
struct tm broken_time;




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
 		printf("players found: %d\n",row_count);
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
			int hours_since_logout = calc_hours(PQgetvalue(res,i,1));
			
			/* >= 0 means the first run of dailybot will update stamina, is it good? */
			/* (>= 0 is always true) */
			if (hours_since_logout >= 0 && hours_since_logout < 24) 
				newstamina = 1;
				
			
	
			/* stamina recovery depends on the place */
			switch (player_location)
			{
				case LOC_ONLINE:
				{
					printf("Player #%d is online, no recovery!\n", player_id);
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

				default:
					break;
			}

		/* do the actual update */
		if (player_location != LOC_ONLINE) /* don' update online players */
			{
			int old_stamina = player_stamina;
			player_stamina += newstamina;
			if (player_stamina > STAMINA_MAX) /* don't go over STAMINA_MAX */
				player_stamina = STAMINA_MAX;

			/* TODO: database update */
 			char *id = itoa(player_id);
			char *stamina = itoa(player_stamina);
 
			const char *params[2] = {id,stamina};
			PGresult *res;
			res = PQexecPrepared(conn, "update_stamina", 2, params, NULL, NULL, 0);
			if (PQresultStatus(res) != PGRES_COMMAND_OK)
			{ /* TODO: since it's a cronjob nobody will ever see this error message..*/
				printf("Dailybot database update failed!\n");
			}

			printf("Player #%d stamina updated from %d to %d\n",player_id, old_stamina, player_stamina);
			}
		} /* don't update, since stamina is at max already */
		else
			printf("Player #%d stamina is already at STAMINA_MAX (%d)\n",player_id,STAMINA_MAX);
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

int calc_hours(char *asciidate)
{
/* this function calculates how many hours since last logout */

// TODO: don't use struct tm but time_t for seconds..
// 1. convert the ascii presentation of last_logout from database to a struct tm
// WARNING: UGLY CODE AHEAD
char year[4] = "";
year[0] = asciidate[0];
year[1] = asciidate[1];
year[2] = asciidate[2];
year[3] = asciidate[3];
year[4] = '\0';
char *year_ptr = year;
int year_int = atoi(year_ptr);

char month[1] = "";
month[0] = asciidate[5];
month[1] = asciidate[6];
month[2] = '\0';
char *mon_ptr = month;
int mon_int = atoi(mon_ptr);

char day[2] = "";
day[0] = asciidate[8];
day[1] = asciidate[9];
day[2] = '\0';
char *day_ptr = day;
int day_int = atoi(day_ptr);

char hour[2] = "";
hour[0] = asciidate[11];
hour[1] = asciidate[12];
hour[2] = '\0';
char *hour_ptr = hour;
int hour_int = atoi(hour_ptr);

/* debug 
printf("year: %d\n",year_int);
printf("month: %d\n",mon_int);
printf("day: %d\n",day_int);
printf("hour: %d\n",hour_int);
printf("date here: %s\n",asciidate);
// 2012-06-27 22:31:01
*/



/* struct tm needs..
year as year from 1900
month as months from january 
*/
year_int -= 1900;
mon_int -= 1;

/* calculate how many total hours time has (forget minutes and so on) */
struct tm logout = { 0,0,hour_int,day_int,mon_int,year_int,0,0,0,0,0 };
time_t logout_seconds = mktime(&logout);
time_t epoch = time(NULL);
time_t since = difftime(epoch,logout_seconds);
since = since/60/60; /* calculate how many FULL HOURS since last_logout, (remember, dailybot is run every even hour, so it's a bit off */

/* return how many full hours has passed */
return since;
}
