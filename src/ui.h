#include <ncurses.h>
#include "player.h"

WINDOW *mainbw, *gamebw, *gamew, *commandw;

void init_ui();
void ncurs_location(Player player);
void ncurs_stats(Player player);
