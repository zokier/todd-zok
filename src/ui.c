#include <ncurses.h> 
#include <locale.h> 
#include <string.h> 
#include <syslog.h> 
#include "ui.h" 
#include "globals.h" 
#include "skills.h"
#include "character.h"

extern Character enemy;

WINDOW *mainbw, *gamebw, *gamew, *commandw, *skillsw;
void init_ui()
{
	// locale needs to be initialized for ncurses
	// "" sets "native" locale
	char *locale = setlocale(LC_ALL, "");
	if (locale == NULL)
	{
		syslog(LOG_WARNING, "setlocale failed, continuing anyways");
	}

	/* ncurses init stuff */
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);	/* Function Keys  - problems with some terminals */
	curs_set(0); 		/* 0 = invisible, 1 = normal, 2 = really visible */
	refresh();

	/* checks that terminal size is big enough */
	int y_size, x_size;
	getmaxyx(stdscr, y_size, x_size);
	if (y_size < 24 || x_size < 80)
	{
		wprintw(stdscr, "This program needs 80x24 characters of screen 	size to run.\n");
		wprintw(stdscr, "You currently have: %dx%d\n", x_size, y_size);
		wprintw(stdscr, "Enlarge your screen and press a key or this program might segfault.\n");
		getch();
		werase(stdscr);
	}

	/* checks that terminal supports color */

	if (has_colors() == FALSE)
	{
		wprintw(stdscr, "Your terminal does not seem to support color!\n");
		wprintw(stdscr, "The game will try to use color whenever possible\n");
		wprintw(stdscr, "You might experience problems\n");
		wprintw(stdscr, "Press a key to continue\n");
		getch();
	}

	/*
	// OLD CODE FOR COLOR STUFF, NOT USED AT THE MOMENT
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_BLACK);
	syntax for color usage:
	attron(COLOR_PAIR(1));
	wprintw(stdscr, "color");
	attroff(COLOR_PAIR(1));
	*/

	/* initialization of windows */
	/* naming reference:
	   mainbw = outer area, "box"
	   gamebw  = main game "box"
	   gamew = main game area
	   commandw = command listing window
           skillsw = skills listing window
	   */

	/* TODO: calculate window sizes */
	mainbw = newwin(24, 80, 0, 0);
	box(mainbw, 0, 0);
	wattron(mainbw, A_BOLD);
	mvwprintw(mainbw, 0, 2, "Tales of Deep Dungeons");
	wattroff(mainbw, A_BOLD);
	wrefresh(mainbw);
	doupdate();
	commandw = subwin(mainbw, 18, 20, 3, 1);
	wborder(commandw, 1, 0, 0, 1, 1, 1, 1, 1);
	skillsw = subwin(mainbw, 8, 20, 11, 1);

	gamebw = subwin(mainbw, 18, 58, 1, 20);
	wattron(gamebw, A_UNDERLINE);
	mvwprintw(gamebw, 0, 2, "TODO: Location info here");
	wattroff(gamebw, A_UNDERLINE);
	wrefresh(gamebw);
	gamew = subwin(gamebw, 16, 56, 3, 22);
	wrefresh(mainbw);
	wrefresh(commandw);
	wrefresh(gamew);
	wrefresh(skillsw);
	doupdate();

}

void ncurs_location_desc() {
	wclear(gamew);
	mvwprintw(gamew, 1, 0, player.location->description);
	wrefresh(gamew);
}

void ncurs_commands() {
	wclear(commandw);
	for (size_t i = 0; i < player.location->action_count; i++)
	{
		if (player.location->actions[i].description != NULL)
		{
			wprintw(commandw, "%s", player.location->actions[i].description_prefix);
			waddch(commandw, player.location->actions[i].description[0] | A_BOLD | A_UNDERLINE);
			wprintw(commandw, "%s\n", player.location->actions[i].description + 1);
		}
	}
	wrefresh(commandw);
}

void ncurs_msg(char *buffer) {
	wclear(gamew);
	wprintw(gamew, "\n%s\n", buffer);
	wrefresh(gamew);
}

/*	Prints descriptions at current player position */
void ncurs_location()
{
	ncurs_location_desc(player);
	ncurs_commands(player);
}

/* TODO: it's stupid to have 2 functions that differ so slightly. Make this one function */
void ncurs_fightstats(WINDOW *window) {
	/* werase makes it possible to cleanly go from 10 to 9 hitpoints (second number cleared) */
	/* TODO: if it flickers, only clean the number part */
	werase(window); 
	box(window,0,0);

	mvwprintw(window,1,1,"Health: %d/%d",player.health,player.max_health);
	mvwprintw(window,3,1,"Wood:   %d",player.elements[ELEM_WOOD]);
	mvwprintw(window,4,1,"Fire:   %d",player.elements[ELEM_FIRE]);
	mvwprintw(window,5,1,"Earth:  %d",player.elements[ELEM_EARTH]);
	mvwprintw(window,6,1,"Metal:  %d",player.elements[ELEM_METAL]);
	mvwprintw(window,7,1,"Water:  %d",player.elements[ELEM_WATER]);

	if(player.elemental_type == player.skill->dmg_type && player.elemental_type == player.weapon->dmg_type)
                mvwprintw(window,10,1,"ALIGNED, POWERFUL!");
	wrefresh(window); 
}

void ncurs_fightstats_enemy(WINDOW *window) {
	/* werase makes it possible to cleanly go from 10 to 9 hitpoints (second number cleared) */
	/* TODO: if it flickers, only clean the number part */
	werase(window); 

	box(window,0,0);
	mvwprintw(window,1,1,"Health: %d",enemy.health);
	mvwprintw(window,2,1,"TYPE:   %s",element_names[enemy.elemental_type]);
	mvwprintw(window,3,1,"Wood:   %d",enemy.elements[ELEM_WOOD]);
	mvwprintw(window,4,1,"Fire:   %d",enemy.elements[ELEM_FIRE]);
	mvwprintw(window,5,1,"Earth:  %d",enemy.elements[ELEM_EARTH]);
	mvwprintw(window,6,1,"Metal:  %d",enemy.elements[ELEM_METAL]);
	mvwprintw(window,7,1,"Water:  %d",enemy.elements[ELEM_WATER]);
	mvwprintw(window,8,1,"skill:  %s",enemy.skill->name);
	mvwprintw(window,9,1,"weapon: %s",enemy.weapon->name);

	if(enemy.elemental_type == enemy.skill->dmg_type && enemy.elemental_type == enemy.weapon->dmg_type)
		mvwprintw(window,10,1,"ALIGNED, POWERFUL!");	

	wrefresh(window); 
}
