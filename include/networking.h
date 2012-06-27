#ifndef NETWORKING_H
#define NETWORKING_H

// port numbers are unused as of now
#define PUSH_PORT 5558
#define CHAT_PORT 5559

#define CHATMSG_PREFIX "chat"
#define DEBUGMSG_PREFIX "debug"

void send_chat_msg(char *msg, size_t len);
void send_dbg_msg(char *msg, size_t len);
void send_msg(char *prefix, size_t prefix_len, char *msg, size_t len);
void zmq_sendmsg(char *buf, int buf_len);

char *try_recv_chatmsg();

#endif //NETWORKING_H
