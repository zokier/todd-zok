#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include <ncurses.h>

bool todd_getchar(unsigned char *c);
bool todd_getline(char **line, size_t *len, WINDOW *echowindow);

#endif //INPUT_H
