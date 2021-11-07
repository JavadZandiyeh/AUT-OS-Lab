"""Server for multithreaded (asynchronous) chat application."""
from socket import AF_INET, socket, SOCK_STREAM
from _thread import *

# handling a single client connection
def client_handler(client):  # Takes client socket as argument.
    """client commands:
        join [groupId]
        send [groupId] [message]
        leave [groupId]
        quit
    """
    try:
        name = client.recv(BUFSIZ).decode("utf8")
        print(name, ": login to app")
        clinets[name] = client
        while True:
            command = client.recv(BUFSIZ).decode("utf8")
            command = command.split()

            if command[0] == "quit":
                print(name, ": quit from app")
                client.close()
                break

            elif command[0] == "join":
                if name not in groups[command[1]]:
                    groups[command[1]].append(name)
            
            elif command[0] == "send":
                if name in groups[command[1]]:
                    for c_name in groups[command[1]]:
                        message = str(name + ": " + ' '.join(command[2:]))
                        clinets[c_name].send(bytes(message, "utf8"))            
            
            elif command[0] == "leave":
                if name in groups[command[1]]:
                    groups[command[1]].remove(name)

            print(groups)
    finally:
        client.close()

      
groups = {"1":[], "2":[], "3":[]} # some groups of users
clinets = {}
BUFSIZ = 1024 # buffer size

"""create a socket
    AF_INET : address domain of the socket
    SOCK_STREAM : means that data or characters are read in a continuous flow"""
SERVER = socket(AF_INET, SOCK_STREAM)

# try to bind an address
try:
    # host = '127.0.0.1, port = 1234, address = (host, port)
    SERVER.bind(('127.0.0.1', 1234))
except socket.error as e:
    print(str(e))

# listen for clinet request
print("Waiting for connection...")
SERVER.listen()

try:
    while True:
        client, client_address = SERVER.accept()
        start_new_thread(client_handler, (client, ))
finally:
    SERVER.close()