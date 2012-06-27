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

bool todd_getchar(char *c)
{
	zmq_pollitem_t items [2];
	items[0].socket = chat_socket;
	items[0].events = ZMQ_POLLIN;
	items[1].socket = NULL;
	items[1].fd = fileno(stdin);
	items[1].events = ZMQ_POLLIN;
	/* Poll for events indefinitely */
	do
	{
		int rc = zmq_poll (items, 2, -1);
		if (rc < 0)
		{
			return false;
		}
		if (items[0].revents & ZMQ_POLLIN)
		{
			char *msg = try_recv_chatmsg();
			parse_and_print_chatmsg(msg);
			free(msg);
		}
	} while (!(items[1].revents & ZMQ_POLLIN));
	char dummy;
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
	chat_typing = 1;
	bool ret = false;
	size_t buf_len = 20;
	*line = malloc(buf_len);
	*len = 0;
	char c;
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
	ret = true;

cleanup:
	werase(input_win);
	wrefresh(input_win);
	chat_typing = 0;
	return ret;
}
