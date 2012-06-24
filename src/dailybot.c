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

void initdb();
void hourly();
void daily();
void stamina();
void random_daily();
void time_dependent();
void quests();


int main(int argc,char *argv[]) {
initdb();
if (argc != 1) { /* there's arguments */
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
	* configuration variable MAX_STAMINA
*/
}

void initdb() {
/* TODO: init database connection */
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
