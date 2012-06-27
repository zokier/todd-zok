#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <stdarg.h>
#include <zmq.h>

#include "networking.h"
#include "globals.h"

/* flow of messages: send_msg starts formatting, parse_PREFIX formats the correct message, zmq_sendmsg sends it */

void send_msg(int len, const char *fmt, ...) 
{
	/* TODO: rewrite the code, at the moment variable argument list usage is a bubblegum hack */
	/* at the moment this function doesn't recognise if an argument is missing */
	va_list argp;
	va_start(argp, fmt);

	char *prefix = fmt;
	char *buf; /* first argument */
	char *buf2; /* second argument */
	/* it's a debug message */
        if (strcmp(prefix, DEBUGMSG_PREFIX) == 0) 
	{ /* *buf is not malloc'ed, is this bad? */
		buf = va_arg (argp, char *);
		buf = parse_debugmsg(buf);
        }
	
	/* it's a chat message */
        if (strcmp(prefix, CHATMSG_PREFIX) == 0) 
	{ /* *buf is not malloc'ed, is this bad? */
		buf = va_arg (argp, char *);
		buf2 = va_arg (argp, char *);
		buf = parse_chatmsg(buf,buf2);
        }

	/* send the message to the network */
	zmq_sendmsg(buf, (strlen(buf) + 1)); /* lazy coding, no idea where that +1 comes from */
	free(buf);
	va_end(argp);
}

char *parse_debugmsg(char *buffer) {
/* put the complete message to buf and length to buf_len */
size_t  buf_len = strlen(DEBUGMSG_PREFIX) + 1 + strlen(buffer) +1; /* + 1 = |, the second +1 is ??? */
char *buf = malloc(buf_len);
snprintf(buf, buf_len, "%s|%s", DEBUGMSG_PREFIX, buffer);
return buf;
}

char *parse_chatmsg(char *nick, char *line) {
/* put the complete message to buf and length to buf_len */
size_t  buf_len = strlen(CHATMSG_PREFIX) + 2 + strlen(nick) + strlen(line) +1; /* + 1 = |, the second +1 is ??? */

char *buf = malloc(buf_len);
snprintf(buf, buf_len, "%s|%s|%s", CHATMSG_PREFIX, nick,line);
return buf;
}
/* handles the actual sending of messages */
void zmq_sendmsg(char *buf, int buf_len) {
	size_t blob_len = strnlen(buf, buf_len);
	int rc;
	zmq_msg_t zmq_message;
	zmq_msg_init_size (&zmq_message, blob_len);
	char *blob = zmq_msg_data(&zmq_message);
	memcpy(blob, buf, blob_len);
	rc = zmq_send (push_socket, &zmq_message, 0);
	if (rc != 0)
		syslog(LOG_ERR, "send_msg failure!");
	zmq_msg_close (&zmq_message);

}


char *try_recv_chatmsg()
{
    zmq_msg_t message;
    zmq_msg_init (&message);
    if (zmq_recv (chat_socket, &message, ZMQ_NOBLOCK))
        return (NULL);
    int size = zmq_msg_size (&message);
    char *string = malloc (size + 1);
    memcpy (string, zmq_msg_data (&message), size);
    zmq_msg_close (&message);
    string [size] = 0;
    return (string);
}
