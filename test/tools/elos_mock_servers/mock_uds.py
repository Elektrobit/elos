import socket
import struct
import os

socket_path = '/tmp/elos_unix_socket'
version = 1
reply_mc = int("0x82", 16)
reply_msg = '{"error":"unix domain socket not implemented"}'
reply_data = reply_msg.encode('utf-8') + b'\0'
reply = struct.pack("<bBh", version, reply_mc, len(reply_data)) + bytearray(reply_data)

# remove the socket file if it already exists
try:
    os.unlink(socket_path)
except OSError:
    if os.path.exists(socket_path):
        raise

# Create the Unix socket server
server = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)

# Bind the socket to the path
server.bind(socket_path)

# Listen for incoming connections
server.listen(1)

# accept connections
print('Server is listening for incoming connections...')
connection, client_address = server.accept()

try:
    print('Connection from', str(connection).split(", ")[0][-4:])

    # receive data from the client
    while True:
        data = connection.recv(1024)
        if not data:
            break
        v, m, l = struct.unpack("<bBh", data[:4])
        data = data[4:4+l]
        data = data.decode('utf-8')
        data = data.strip()
        print('Received data:', data)

        # Send a response back to the client
        # response = '{"error":"unix domain socket not implemented"}'
        connection.sendall(reply)
finally:
    # close the connection
    connection.close()
    # remove the socket file
    os.unlink(socket_path)
