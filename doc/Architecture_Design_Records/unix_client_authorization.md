# Design Decisions - unix client authorization

## Problem

Elos is designed to connect to different clients. How to ensure only authorized client can publish or subscribe
certain events?

## Considered Alternatives

### 1) Use the NETLINK_SOCK_DIAG Interface

Use the NETLINK_SOCK_DIAG protocol to obtain the credentials of the process connected to the socket.

How It Works:
Send a Netlink request with the AF_UNIX family.
Parse the unix_diag_msg response to extract the UNIX_DIAG_PEER attribute, which contains the peer PID.

Pros: This approach has been already implemented for tcp/ip client so it only needs to be adapted.

Cons: This solutions has requirements on the used kernel: The CONFIG_NETLINK option has to be enabled.


### 2) Use option SO_PEERCRED with getsockopt

Use the getsockopt() system call with the SO_PEERCRED option on the Unix domain socket to obtain the credentials of the process connected to the socket.

How it works:
getsockopt(sock_fd, SOL_SOCKET, SO_PEERCRED, &credentials, &len)

Pros: This approach is very easy to implement.

Cons: A connection has to be established and this solution does not track the actual state of the sending process.

### 3) Use option SO_PEERCRED with setsockopt

The SO_PASSCRED socket option enables a process to receive credentials of the peer process as data when the peer sends a message.

How it works:
setsockopt(sock_fd, SOL_SOCKET, SO_PASSCRED, &enable, sizeof(enable))
Retrieve a message with recvmsg system call and extract the credentials from ancillary data.

Pros: This approach is easy to implement and dynamically identifies the sender for every message.

Cons: Solution would need a refactoring of existing message handling code (use of recvmsg instead of recv). Also a connection has to be established.

### 4) Parse /proc/net/unix

Parse /proc/net/unix to find the inode of the socket, then map it to a process by finding the inode in the corresponding pid folder in /proc/[pid]/fd/.

How it works:
Extract the socket’s inode from /proc/net/unix.
Iterate through /proc/[pid]/fd/ to find file descriptors linked to that inode. /proc/[pid]/status contains UID and GUID.

Pros: Solution works without a connection and can identify all processes on the socket.

Cons: Approach needs elevated permissions and is inefficient
as it has to loop to all processes.

## Decision

Option 2 is chosen.
