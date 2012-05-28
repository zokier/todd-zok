#ifndef MESSAGE_H
#define MESSAGE_H

typedef enum Message_type Message_type;
enum Message_type
{
	MSG_CHATMSG
};

#define CHATMSG_FMT "S(ss)"
typedef struct Chatmsg Chatmsg;
struct Chatmsg
{
	char *from;
	char *msg;
};

typedef struct Message Message;
struct Message
{
	Message_type type;
	uint32_t payload_size;
	void *payload;
};

#endif //MESSAGE_H
