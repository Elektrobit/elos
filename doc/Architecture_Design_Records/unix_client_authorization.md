# Design Decisions - <headline>

## Problem

Elos is designed to connect to different clients. How to ensure only authorized client can publish or subscribe
certain events?

## Considered Alternatives

### 1) Use the NETLINK_SOCK_DIAG Interface

Use the NETLINK_SOCK_DIAG protocol to obtain the credentials of the process connected to the socket.

How It Works:
Send a Netlink request with the AF_UNIX family.
Parse the unix_diag_msg response to extract the UNIX_DIAG_PEER attribute, which contains the peer PID.

*pros*
* robust and efficient

*cons*
* complex to implement
* Kernel hast to support Netlink


### 2) Use option SO_PEERCRED with getsockopt

Use the getsockopt() system call with the SO_PEERCRED option on the Unix domain socket to obtain the credentials of the process connected to the socket.

How it works:
getsockopt(sock_fd, SOL_SOCKET, SO_PEERCRED, &credentials, &len)

*pros*
* very easy to implement

*cons*
* fixed to the peer connected at the socket level
* connection has to be established


### 3) Use option SO_PEERCRED with setsockopt

The SO_PASSCRED socket option enables a process to receive credentials of the peer process as data when the peer sends a message.

How it works:
setsockopt(sock_fd, SOL_SOCKET, SO_PASSCRED, &enable, sizeof(enable))
Retrieve a message with recvmsg system call and extract the credentials from ancillary data.

*pros*
* easy to implement
* Dynamically identifies the sender for every message

*cons*
* existing code has to be refactored
* connection has to be established

### 4) Parse /proc/net/unix

Parse /proc/net/unix to find the inode of the socket, then map it to a process by finding the inode in the corresponding pid folder in /proc/[pid]/fd/.

How it works:
Extract the socket’s inode from /proc/net/unix.
Iterate through /proc/[pid]/fd/ to find file descriptors linked to that inode. /proc/[pid]/status contains UID and GUID.

*pros*
* works without connection
* can identify all processes on the socket

*cons*
* need elevated permissions
* inefficient


## Decision

Option 1 is already implemented for tcp/ip client so it only needs to be adapted. The mean disadvantage is that this solutions has requirements on the used kernel: The CONFIG_NETLINK option has to be enabled.
Option 2 is very easy to implement but would not track the actual state of the sending process.
Option 3 would need a refactoring of existing message handling code (use of recvmsg instead of recv). Both options depends on an established connection, which is seldom a problem in a daemon-client setup. At the most it could be a debug problem.
Option 4 could work but is very inefficient as it has to loop to all processes.

So overall option 2 and 3 are most suitable. Option 2, use option SO_PEERCRED with getsockopt, is chosen as it provides the easiest way of implementation. If the current socket status is needed after all, option 3 could be taken.
