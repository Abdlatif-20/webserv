#!/usr/local/bin/python3
from socket import *
from sys import *
import selectors
def	conn(host, port, i):
	try:
		fd = socket(AF_INET, SOCK_STREAM)
		fd.connect((host, port))
		fd.setblocking(False)
		fd.send("GET / HTTP/1.1\r\n".encode())
		fd.send("host:sss\r\n".encode())
		fd.send("CONTENT-LENGTH: 85\r\n".encode())
		fd.send("connection:closed\r\n".encode())
		fd.send("\r\n".encode())
		return fd
	except Exception as err:
		print("errr :" , err)

if __name__ == "__main__":
	if len(argv) != 4:
		exit(1)
	l = selectors.DefaultSelector()
	for item in range(int(argv[3])):
		s = conn(argv[1], int(argv[2]), item)
		if s:
			l.register(s, selectors.EVENT_READ | selectors.EVENT_WRITE)
	kkkk = True
	while kkkk:
		for k, m in l.select(1):
			con = k.fileobj
			if m & selectors.EVENT_READ:
				data = con.recv(2048)
				print(data.decode(), end='')
