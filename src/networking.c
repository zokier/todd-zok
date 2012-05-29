#define _GNU_SOURCE //mempcpy
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <zmq.h>

#include "networking.h"
#include "globals.h"

void send_chatmsg(char *msg, size_t len)
{
	size_t blob_len = sizeof(CHATMSG_PREFIX)-1 + len;
	int rc;
	zmq_msg_t zmq_message;
	zmq_msg_init_size (&zmq_message, blob_len);
	memcpy(mempcpy(zmq_msg_data(&zmq_message), CHATMSG_PREFIX, sizeof(CHATMSG_PREFIX)-1), msg, len);
	rc = zmq_send (push_socket, &zmq_message, 0);
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
