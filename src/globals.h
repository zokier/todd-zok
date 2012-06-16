#ifndef GLOBALS_H
#define GLOBALS_H

#include "player.h"
#include "enemy.h"
#include <libpq-fe.h>

// ugly globals go here
extern int playing;
extern void *push_socket;
extern void *chat_socket;
extern void *zmq_context;
extern Player player;
extern PGconn *conn;

extern Weapons weapons_list[];
extern Weapons weapons_enemy[];
extern Enemy enemylist[];
extern Skills skills_list[];

#endif
