#ifndef LOCATION_H
#define LOCATION_H

#include "action.h"

typedef struct Location Location;
struct Location {
	char *description;
	size_t action_count; //number of items in actions array
	Action actions[];
};

#endif //LOCATION_H
