#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>

char todd_getchar(bool *ok);
bool todd_getline(char **line, size_t *len);
void reset_terminal_mode();
void set_terminal_mode();

#endif //INPUT_H
