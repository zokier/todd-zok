#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <stdarg.h>
#include <zmq.h>

#include "networking.h"
#include "globals.h"

void send_chat_msg(char *msg, size_t len)
{
	size_t buf_len = len + NAME_MAX_LENGTH + sizeof('|') + sizeof('\0');
	char *buf = malloc(buf_len);
	snprintf(buf, buf_len, "%s|%s", player.name, msg);
	send_msg(CHATMSG_PREFIX, sizeof(CHATMSG_PREFIX), buf, buf_len);
	free(buf);
}

void send_dbg_msg(char *msg, size_t len)
{
	send_msg(DEBUGMSG_PREFIX, sizeof(DEBUGMSG_PREFIX), msg, len);
}

void send_msg(char *prefix, size_t prefix_len, char *msg, size_t len)
{
	size_t buf_len = len + prefix_len + sizeof('|');
	char *buf = malloc(buf_len);
	snprintf(buf, buf_len, "%s|%s", prefix, msg);
	zmq_sendmsg(buf, buf_len);
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
