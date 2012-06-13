#include <stdio.h>
#include <stdbool.h>

#include "events.h"
#include "locations.h"

extern void set_player_location(Location* loc);

int ev_old_man()
{
	set_player_location(&loc_ev_oldman);
	return 1;
}

