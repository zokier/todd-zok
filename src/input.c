#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <zmq.h>
#include <ncurses.h>
#include "globals.h"
#include "ui.h"
#include "networking.h"

extern int chat_typing;
extern int toggle_chat;

void parse_and_print_chatmsg(char *msg)
{
	char *prefix = strtok_r(NULL, "|", &msg);

	if (strcmp(prefix, "chat") == 0) {
		char *nick = strtok_r(NULL, "|", &msg);
		ncurs_log_chatmsg(msg, nick);
	}
}

bool todd_getchar(unsigned char *c)
{
	zmq_pollitem_t items [3];
	items[0].socket = chat_socket;
	items[0].events = ZMQ_POLLIN;
	items[1].socket = NULL;
	items[1].fd = fileno(stdin);
	items[1].events = ZMQ_POLLIN;
	items[2].socket = party_socket;
	items[2].events = ZMQ_POLLIN;
	/* Poll for events indefinitely */
	do
	{
		int rc = zmq_poll (items, 3, -1);
		if (rc < 0)
		{
			return false;
		}
		if (items[0].revents & ZMQ_POLLIN)
		{
			char *msg = try_recv_msg(chat_socket);
			parse_and_print_chatmsg(msg);
			free(msg);
		}
		if (items[2].revents & ZMQ_POLLIN)
		{
			char *msg = try_recv_msg(party_socket);
			// TODO real prefix parsing. avoid buffer overrun
			parse_and_print_chatmsg(msg+11); // skip partyprefix
			free(msg);
		}
	} while (!(items[1].revents & ZMQ_POLLIN));
	unsigned char dummy;
	if (c == NULL) c = &dummy;

	/* if no events from network, return a local getch */
	int rc = read(fileno(stdin), c, sizeof(char));

	/* toggle chat window */
	if (rc == 1)
	{
		if (*c == '\t')
		{ 
			if (chat_typing == 0) /* if not typing yet, goto chat toggle */
			{
				ncurs_chat(player);
			}
			else
			{
				chat_typing = 0; /* if typing already, cancel it */
				ncurs_chat(player);
			}
		}
		return true;
	}
	else	
	{
		return false;
	}
}

bool todd_getline(char **line, size_t *len)
{
	// curs_set displays a nice cursor for user convenience
	curs_set(1);
	chat_typing = 1;
	bool ret = false;
	size_t buf_len = 20;
	*line = malloc(buf_len);
	*len = 0;
	unsigned char c;
	do
	{
		bool rc = todd_getchar(&c);
		if (!rc || c == '\t')
		{
			free(*line);
			*line = NULL;
			*len = 0;
			goto cleanup;
		}
		if (c == '\b') // it's a backspace, go back a character
		{
			// don't backspace on an empty string or the pointer will cause a segfault
			if ((*len) != 0) 
			{
				// if it's a multibyte (scandic letters, § and so on)
				// len -1 and len -2 have negative values
				int multibyte = 0;
				if ((*line)[*len - 1] < 0)
					multibyte = 1;

				int y,x;
				// get current position of cursor to y and x
				getyx(input_win,y,x);
				// move the cursor left by one
				wmove(input_win, y,x-1);
				// blank it from screen and from buffer
				wechochar(input_win, ' ');
				(*line)[*len] = '\0';
				(*len)--;
				if (multibyte) // there's two chars in buffer, not one
				{
				(*line)[*len] = '\0';
				(*len)--;
				}

				// by calling wechochar, the cursor moves right. move it back
				wmove(input_win, y,x-1);
				wrefresh(input_win);
			}
		}
		else
		{
			if (buf_len <= *len)
			{
				buf_len += 20;
				*line = realloc(*line, buf_len);
			}

			(*line)[*len] = c;
			(*len)++;
			wechochar(input_win, c);
		}

	} while (c != '\r');
	(*len)--; // strip trailing newline
	(*line)[*len] = '\0'; // insert null terminator

	curs_set(0);
	if (*len == 0) // it's an empty string..
		return false;
	// else, return true
	ret = true;

cleanup:
	werase(input_win);
	wrefresh(input_win);
	chat_typing = 0;
	curs_set(0);
	return ret;
}
