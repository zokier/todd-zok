#ifndef GLOBALS_H
#define GLOBALS_H

#include <libpq-fe.h>
#include <ncurses.h>
#include "character.h"

// ugly globals go here
extern int playing;
extern void *push_socket;
extern void *chat_socket;
extern void *zmq_context;
extern Character player;
extern Character enemy;
extern PGconn *conn;

extern Weapons weapons_list[];
extern Weapons weapons_enemy[];
extern Character enemylist[2][ENEMY_COUNT];
extern Skills skills_list[];
extern WINDOW *game_win;
extern WINDOW *input_win;
extern WINDOW *background_win;
extern int y_size;
extern int gamew_logw_sep;
#endif
