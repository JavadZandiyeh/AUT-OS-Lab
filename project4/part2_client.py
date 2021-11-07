from socket import AF_INET, socket, SOCK_STREAM
from _thread import *

def read_server_message(CLIENT):
    while True:
        Response = CLIENT.recv(BUFSIZ)
        if not Response:
            pass
        else:
            print(Response.decode('utf-8'))



CLIENT = socket(AF_INET, SOCK_STREAM)
print('Waiting for connection')

try:
    CLIENT.connect(('127.0.0.1', 1234))
except socket.error as e:
    print(str(e))

BUFSIZ = 1024 # buffer size

# send name to server
Input = input("what is your name? ")
CLIENT.send(str.encode(Input))

try:
    # check for response
    start_new_thread(read_server_message, (CLIENT, ))
    while True:
        # send command
        Input = input()
        CLIENT.send(str.encode(Input))
        if Input == "quit":
            break
finally:
    CLIENT.close()