#ifndef GLOBALS_H
#define GLOBALS_H

#include "player.h"

// ugly globals go here
extern int playing;
extern void *push_socket;
extern void *chat_socket;
extern void *zmq_context;
extern Player player;

#endif
