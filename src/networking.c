#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <stdarg.h>
#include <zmq.h>

#include "networking.h"
#include "globals.h"

Message create_chat_msg(char *body, size_t len)
{
	Message msg;
	size_t buf_len = sizeof(CHATMSG_PREFIX) + sizeof('|') + NAME_MAX_LENGTH + sizeof('|') + len + sizeof('\0');
	char *buf = malloc(buf_len);
	msg.body_len = snprintf(buf, buf_len, "%s|%s|%s", CHATMSG_PREFIX, player.name, body) + 1; // +1 null terminator
	msg.body = buf;
	return msg;
}

Message create_ctrl_msg(char *body, size_t len)
{
	Message msg;
	size_t buf_len = sizeof(CTRLMSG_PREFIX) + sizeof('|') + len + sizeof('\0');
	char *buf = malloc(buf_len);
	msg.body_len = snprintf(buf, buf_len, "%s|%s", CTRLMSG_PREFIX, body) + 1; // +1 null terminator
	msg.body = buf;
	return msg;
}

bool send_msg(Message msg)
{
	return zmq_sendmsg(msg.body, msg.body_len);
}

Message wrap_as_partymsg(Message msg)
{
	Message wrapped;
	size_t buf_len = msg.body_len + 12; // TODO no hardcoding/magic numbers
	char *buf = malloc(buf_len);
	wrapped.body_len = snprintf(buf, buf_len, "p%010d|%s", player_party.id, msg.body);
	wrapped.body = buf;
	del_msg(msg);
	return wrapped;
}

void del_msg(Message msg)
{
	free(msg.body);
	msg.body = NULL;
}

/* handles the actual sending of messages */
bool zmq_sendmsg(char *buf, int buf_len) {
	bool ret = true;
	size_t blob_len = strnlen(buf, buf_len);
	int rc;
	zmq_msg_t zmq_message;
	zmq_msg_init_size (&zmq_message, blob_len);
	char *blob = zmq_msg_data(&zmq_message);
	memcpy(blob, buf, blob_len);
	rc = zmq_send (push_socket, &zmq_message, 0);
	if (rc != 0)
	{
		ret = false;
		syslog(LOG_ERR, "zmq_sendmsg failure: %s", zmq_strerror(errno));
	}
	zmq_msg_close (&zmq_message);
	return ret;
}

char *try_recv_msg(void *sock)
{
    zmq_msg_t message;
    zmq_msg_init (&message);
    if (zmq_recv (sock, &message, ZMQ_NOBLOCK))
	{
		syslog(LOG_WARNING, "ZMQ receive failure: %s", zmq_strerror(errno));
        return (NULL);
	}
    int size = zmq_msg_size (&message);
    char *string = malloc (size + 1);
    memcpy (string, zmq_msg_data (&message), size);
    zmq_msg_close (&message);
    string [size] = 0;
    return (string);
}
