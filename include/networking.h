#ifndef NETWORKING_H
#define NETWORKING_H

// port numbers are unused as of now
#define PUSH_PORT 5558
#define CHAT_PORT 5559

/* all net msg prefixes must be 4 letters */
#define CHATMSG_PREFIX "chat"
#define DEBUGMSG_PREFIX "debu"

void send_msg(char *prefix, char *msg, size_t len);
char *try_recv_chatmsg();

#endif //NETWORKING_H
