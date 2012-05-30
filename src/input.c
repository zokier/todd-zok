#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <termios.h>
#include <zmq.h>
#include "networking.h"
#include "globals.h"

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
			puts("Suddenly you hear a voice appear from thin air, saying:");
			puts(msg+(sizeof(CHATMSG_PREFIX)-1));
			free(msg);
		}
	} while (!(items[1].revents & ZMQ_POLLIN));
	return (read(fileno(stdin), c, sizeof(char)) > 0);
}

struct termios orig_termios;
void reset_terminal_mode()
{
    tcsetattr(0, TCSANOW, &orig_termios);
}

void set_terminal_mode()
{
    struct termios new_termios;

    /* take two copies - one for now, one for later */
    tcgetattr(0, &orig_termios);
    memcpy(&new_termios, &orig_termios, sizeof(new_termios));

    /* register cleanup handler, and set the new terminal mode */
    atexit(reset_terminal_mode);
	new_termios.c_lflag &= ~ICANON;
	new_termios.c_lflag &= ~ECHO;
    tcsetattr(0, TCSANOW, &new_termios);
}

bool todd_getline(char **line, size_t *len)
{
	// TODO poll network (see todd_getchar)
	reset_terminal_mode();
	ssize_t line_len = getline(line, len, stdin);
	set_terminal_mode();
	if (line_len < 0)
	{
		return false;
	}
	*len = line_len;
	return true;
}
