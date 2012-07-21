#include "globals.h"

// update party members as given in parameters. Return 1 if succesful, calling function handles printing
// called when joining a party and when leaving a party
int update_party(int party_id, char *player1, char *player2, char* player3)
{
	// player1, player2 and player3 now hold the party member names
	// update the stuff in database
	char *partyid = itoa(party_id);
	const char *params[4] = {partyid, player1, player2, player3};

	PGresult *update_party;
	update_party = PQexecPrepared(conn, "update_party", 4, params, NULL, NULL, 0);
	if (PQresultStatus(update_party) == PGRES_COMMAND_OK)
		return 1;	// succesful, return 1;

PQclear(update_party);
return 0;
}

