#ifndef UI_H
#define UI_H

#include "character.h"

void init_ui();
void ncurs_location();
void ncurs_commands();
void ncurs_skills();
void ncurs_modal_msg(const char *fmt, ...);
void ncurs_log_sysmsg(const char *fmt, ...);
void ncurs_log_chatmsg(char *msg, char *source);
void ncurs_fightwindow(Character *chr, int index);
int ncurs_listselect(char *first_item, size_t stride, size_t count);

#endif
