#include <ncurses.h>
#include "ui.h"
#include "player.h"

extern Player player;
void init_ui()
{
/* ncurses init stuff */
initscr();				
cbreak();
noecho();       
keypad(stdscr, TRUE);	/* Function Keys  - problems with some terminals */
curs_set(0); 		/* 0 = invisible, 1 = normal, 2 = really visible */
refresh();

/* checks that terminal size is big enough */
int y_size,x_size;
getmaxyx(stdscr,y_size,x_size);
if (y_size < 24 || x_size < 80)
	{
	wprintw(stdscr, "This program needs 80x24 characters of screen 	size to run.\n");
	wprintw(stdscr, "You currently have: %dx%d\n",x_size,y_size);
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
	werase(stdscr);
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
*/

/* TODO: calculate window sizes */
mainbw = newwin(24,80,0,0); 		
box(mainbw, 0,0);			
wattron(mainbw,A_BOLD);
mvwprintw(mainbw,0,2,"Tales of Deep Dungeons");
wattroff(mainbw,A_BOLD);
wrefresh(mainbw);

commandw = subwin(mainbw,18,20,1,1);
wborder(commandw,1,0,0,1,1,1,1,1);
mvwprintw(commandw,1,1,"Commands here");

gamebw = subwin(mainbw,18,60,1,20); 
wattron(gamebw,A_UNDERLINE);
mvwprintw(gamebw,0,2,"TODO: Location info here");
wattroff(gamebw,A_UNDERLINE);
wrefresh(gamebw);
gamew = subwin(gamebw,16,58,2,21);
wrefresh(mainbw);
wrefresh(commandw);
wrefresh(gamew);

doupdate();

}

void ncurs_location(Player player) {
wclear(gamew);
mvwprintw(gamew, 1,0, player.location->description);
wrefresh(gamew);
}

void ncurs_commands(Player player) {
wclear(commandw);
for (size_t i = 0; i < player.location->action_count; i++)
	{
//	printf("\t* %s\n", player.location->actions[i].description);
	mvwprintw(commandw,i,0,player.location->actions[i].description);
	}
wrefresh(commandw);
}
