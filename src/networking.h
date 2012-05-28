#ifndef NETWORKING_H
#define NETWORKING_H

void send_chatmsg(const char *from, const char *body);
char *try_recv_chatmsg();

#endif //NETWORKING_H
