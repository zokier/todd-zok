#!/usr/bin/python

import sys
import zmq

context = zmq.Context()
socket = context.socket(zmq.PUSH)
socket.connect("tcp://localhost:5558")
chat_prefix = "chat"
nick = "foo"
msg = "bar"
if len(sys.argv) > 2:
	nick = sys.argv[1]
	msg = " ".join(sys.argv[2:])
socket.send("|".join([chat_prefix,nick,msg]))

