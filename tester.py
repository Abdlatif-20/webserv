from random import randbytes
from sys import *
from socket import *
import string
import random

def gen2(n):
    random_characters = ''.join(random.choices(string.ascii_letters + string.digits, k=n))
    return random_characters


def gen(n):
    s = ""
    for i in randbytes(n):
        s += chr(i)
    return s.encode()
def send(host, port, name, size, chunk):
    try:
        fd = socket(AF_INET, SOCK_STREAM)
        fd.connect((host, port))
        s = open(name , "w")
        headers = "POST / HTTP/1.1\r\n"
        headers += "HOST:sss\r\n"
        headers += "Transfer-Encoding:chunked\r\n"
        headers += "\r\n"
        # fd.send(headers.encode())
        while size > 0:
            b = b""
            nb = 0
            if size > chunk:
                nb = chunk
            else:
                nb = size
            size -= nb
            headers += str(hex(nb))[2:] + "\r\n"
            b = gen(nb)
            headers += b.decode() + "\r\n"
            s.write(b.decode())
        headers += "0\r\n\r\n"
        fd.send(headers.encode())
        s.close()
        fd.close()
    except:
        print("errrrrrr")
if __name__ == "__main__":
    if len(argv) != 5:
        exit(1)
    send(argv[1],int(argv[2]), argv[3], int(argv[4]), int(int(argv[4]) / 4))
    p = "./chunked_" + input()
    f1 = open(argv[3], "r")
    f2 = open(p, "r")
    if f1.read() != f2.read():
        print("file not equal")
    else:
        print("file equal")