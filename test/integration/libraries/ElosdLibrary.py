# SPDX-License-Identifier: MIT
import socket
import struct


class ElosdLibrary(object):
    """ElosdLibrary provides a set of keywords for direct interaction
    with target elosd.
    """
    host = "127.0.0.1"
    port = 54321
    socket = None

    def elosd_connect(self, host, port):
        """Connect to a elosd target.

        | host | ip string to connect to | Default=54321 |
        | port | elosd port | Default=54321 |
        """
        self.host = host
        self.port = port
        try:
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        except socket.error:
            return 1
        self.socket.connect((self.host, int(self.port)))
        return 0

    def elosd_send(self, version, messagecode, messagestring):
        """Send a message to elosd.

        | version | elosd api verion |
        | messagecode | type of message to be sent to elosd |
        | messagestring | message to be sent |
        """
        data = messagestring.encode('utf-8') + b'\0'
        packet = struct.pack("<bbh", int(version), int(messagecode), len(data)) + bytearray(data)
        try:
            self.socket.sendall(packet)
        except socket.error:
            return 1
        return 0

    def elosd_receive(self):
        """Receive a response to sent message from elosd

        """
        try:
            response = self.socket.recv(4)
        except socket.error:
            return 1

        try:
            rversion, rmessagecode, rlen = struct.unpack("<bbh", response)
        except struct.error:
            return 1

        rdata = self.socket.recv(rlen)[:-1]
        return rdata

    def elosd_disconnect(self):
        """Disconnect from elosd target.

        """
        try:
            self.socket.close()
        except socket.error:
            return 1
        return 0
