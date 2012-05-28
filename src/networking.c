#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <zmq.h>
#include <tpl.h>

#include "message.h"

extern void *push_socket;
extern void *sub_socket;

void send_blob(void *addr, size_t len)
{
	int rc;
	zmq_msg_t message;
	zmq_msg_init_size (&message, len);
	memcpy (zmq_msg_data (&message), addr, len);
	rc = zmq_send (push_socket, &message, 0);
	zmq_msg_close (&message);
}

void send_message(Message msg)
{
	tpl_node *tn;
	tpl_bin payload = {msg.payload, msg.payload_size};
	tn = tpl_map("iB", &msg.type, &payload);
	tpl_pack(tn, 0);
	size_t len;
	void *addr;
	tpl_dump(tn, TPL_MEM, &addr, &len);
	send_blob(addr, len);
	free(addr);
	tpl_free(tn);
}

void send_chatmsg(const char *from, const char *body)
{
	Chatmsg chatmsg = { from, body };
	tpl_node *tn;
	tn = tpl_map(CHATMSG_FMT, &chatmsg);
	tpl_pack(tn, 0);
	size_t len;
	void *addr;
	tpl_dump(tn, TPL_MEM, &addr, &len);
	Message msg = { MSG_CHATMSG, len, addr };
	send_message(msg);
	free(addr);
	tpl_free(tn);
}

char *try_recv_chatmsg()
{
	// TODO
    zmq_msg_t message;
    zmq_msg_init (&message);
    if (zmq_recv (sub_socket, &message, ZMQ_NOBLOCK))
        return (NULL);
    int size = zmq_msg_size (&message);
    char *string = malloc (size + 1);
    memcpy (string, zmq_msg_data (&message), size);
    zmq_msg_close (&message);
    string [size] = 0;
    return (string+5);
}
