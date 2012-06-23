#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <zmq.h>

#include "networking.h"
#include "globals.h"

void send_chatmsg(char *msg, size_t len)
{
	size_t buf_len = sizeof(CHATMSG_PREFIX) + len + NAME_MAX_LENGTH + 2;
	char *buf = malloc(buf_len);
	snprintf(buf, buf_len, "%s|%s|%s", CHATMSG_PREFIX, player.name, msg);
	size_t blob_len = strnlen(buf, buf_len);
	int rc;
	zmq_msg_t zmq_message;
	zmq_msg_init_size (&zmq_message, blob_len);
	char *blob = zmq_msg_data(&zmq_message);
	memcpy(blob, buf, blob_len);
	rc = zmq_send (push_socket, &zmq_message, 0);
	zmq_msg_close (&zmq_message);
	free(buf);
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
