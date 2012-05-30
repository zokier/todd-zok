#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>

bool todd_getchar(char *c);
bool todd_getline(char **line, size_t *len);
void reset_terminal_mode();
void set_terminal_mode();

#endif //INPUT_H
