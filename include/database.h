#ifndef DATABASE_H
#define DATABASE_H

void db_player_location(int location);

/* used for database stuff */
#define LOC_ONLINE 		0
#define LOC_OFFLINE_ROOM 	1
#define LOC_OFFLINE_FIELDS 	2
#define LOC_DEAD 		3
#define LOC_DEAD_GRAVEYARD 	4
#define LOC_FAINTED		5
#endif //DATABASE_H
