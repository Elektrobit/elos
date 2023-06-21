# SPDX-License-Identifier: MIT
import socket
from robot.libraries.OperatingSystem import OperatingSystem

class NetworkLibrary(object):
    """NetworkLibrary provides a set of keywords for network communication.
    """

    def is_valid_ipv4(self, address):
        """Check for valid IPv4 address.

        | address | To be verified |
        """
        try:
            socket.inet_pton(socket.AF_INET, address)
        except socket.error:
            return False
        return True

    def is_valid_ipv6(self, address):
        """Check for valid IPv6 address.

        | address | To be verified |
        """
        try:
            socket.inet_pton(socket.AF_INET6, address)
        except socket.error:
            return False
        return True

    def is_valid_ip(self, address):
        """Check for valid IP address (IPv4 or IPv&).

        | address | To be verified |
        """
        return self.is_valid_ipv4(address) or self.is_valid_ipv6(address)

    # For convenience, use RobotFramework OS library
    def ping(self, address, deadline=1, count=1, interval=1.0):
        """Ping a given IP address.

        | address | To be verified |
        | deadline | Default=1 |
        | count | Default=1 |
        | interval | Default=1.0 |
        """
        return OperatingSystem().run_and_return_rc_and_output(
                "ping -c {} -i {} -w {} {}".format(count, interval, deadline, address))
