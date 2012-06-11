#include <ncurses.h>
#include "player.h"
#include "enemy.h"

WINDOW *mainbw, *gamebw, *gamew, *commandw;

void init_ui();
void ncurs_location(Player player);
void ncurs_stats(Player player);
void ncurs_enemy(Enemy enemy);
void ncurs_msg(char *buffer);
