#!/usr/bin/python

import sys
import zmq

context = zmq.Context()
socket = context.socket(zmq.PUSH)
socket.connect("tcp://localhost:5558")
msg = "foo"
if len(sys.argv) > 1:
	msg = " ".join(sys.argv[1:])
socket.send("chat "+msg)

