import socket
import struct

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

#connect to elos
client.connect(("127.0.0.1", 54321))

version = 1

# send a message to be published in elos
publish_code = 2
messagestring = '{"messageCode": 41,"payload":"testEventFiltering"}'

# pack message to be published to byte array according to elos protocol
data = messagestring.encode('utf-8') + b'\0'
packet = struct.pack("<bbh", int(version), int(publish_code), len(data)) + bytearray(data)

# send to target
client.sendall(packet)

# reponse to sent message
response = client.recv(4)

rversion, rmessagecode, rlen = struct.unpack("<bbh", response)

# extracted response message
received_message = client.recv(rlen)
print(received_message.decode('utf-8').strip())

# send a query to retrieve a published message
query_code = 4
querystring = '{"filter":".event.messageCode 8000 EQ"}'

# pack a query message into byte array according to elos protocol
data = querystring.encode('utf-8') + b'\0'
packet = struct.pack("<bbh", int(version), int(query_code), len(data)) + bytearray(data)

client.sendall(packet)

# reponse to sent message
response = client.recv(4)

rversion, rmessagecode, rlen = struct.unpack("<bbh", response)

# extracted response message
received_message = client.recv(rlen)
print(received_message.decode('utf-8').strip())
