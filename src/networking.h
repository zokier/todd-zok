#ifndef NETWORKING_H
#define NETWORKING_H

// port numbers are unused as of now
#define PUSH_PORT 5558
#define CHAT_PORT 5559
#define CHATMSG_PREFIX "chat "

void send_chatmsg(char *msg, size_t len);
char *try_recv_chatmsg();

#endif //NETWORKING_H
