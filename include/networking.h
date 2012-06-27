#ifndef NETWORKING_H
#define NETWORKING_H

// port numbers are unused as of now
#define PUSH_PORT 5558
#define CHAT_PORT 5559

#define CHATMSG_PREFIX "chat"
#define DEBUGMSG_PREFIX "debug"

void send_msg(int len, const char *fmt, ...);
char *parse_debugmsg(int len, char *buffer);
char *parse_chatmsg(int len, char *line);
void zmq_sendmsg(char *buf, int buf_len);

void send_msg(int len, const char *fmt, ...);
char *try_recv_chatmsg();

#endif //NETWORKING_H
