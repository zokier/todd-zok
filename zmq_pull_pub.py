#!/usr/bin/python

import zmq
import time

ctx = zmq.Context()
pull_sock = ctx.socket(zmq.PULL)
pull_sock.bind("tcp://*:5558")
pub_sock = ctx.socket(zmq.PUB)
pub_sock.bind("tcp://*:5559")
while True:
	msg = pull_sock.recv()
	print repr(msg)
	pub_sock.send(msg)

