#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <zmq.h>
#include <ncurses.h>
#include "ui.h"
#include "networking.h"
#include "globals.h"

void parse_and_print_chatmsg(char *msg)
{
	msg += sizeof(CHATMSG_PREFIX);
	char *nick = strtok_r(NULL, "|", &msg);
	ncurs_log_chatmsg(msg, nick);
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
	return (read(fileno(stdin), c, sizeof(char)) > 0);
}

bool todd_getline(char **line, size_t *len)
{
	ncurs_bold_input(1);
	bool ret = false;
	int buf_len = 20;
	*line = malloc(buf_len);
	*len = 0;
	char c;
	do
	{
		if (!todd_getchar(&c))
		{
			free(*line);
			*line = NULL;
			*len = 0;
			goto cleanup;
		}
		if (buf_len <= *len)
		{
			buf_len += 20;
			*line = realloc(*line, buf_len);
		}
		(*line)[*len] = c;
		(*len)++;
		wechochar(input_win, c);
	} while (c != '\r');
	(*len)--; // strip trailing newline
	(*line)[*len] = '\0'; // insert null terminator
	ret = true;
cleanup:
	werase(input_win);
	wrefresh(input_win);
	ncurs_bold_input(0);
	return ret;
}
