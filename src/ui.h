#ifndef UI_H
#define UI_H

#include <ncurses.h>

extern WINDOW *mainbw, *gamebw, *gamew, *commandw;

void init_ui();
void ncurs_location();
void ncurs_commands();
void ncurs_stats();
void ncurs_msg(char *buffer);

#endif
