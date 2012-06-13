#ifndef UI_H
#define UI_H

#include <ncurses.h>

extern WINDOW *mainbw, *gamebw, *gamew, *commandw, *skillsw;

void init_ui();
void ncurs_location();
void ncurs_commands();
void ncurs_stats();
void ncurs_msg(char *buffer);
void ncurs_fightstats(WINDOW *window);
void ncurs_fightstats_enemy(WINDOW *window);
#endif
