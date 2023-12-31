# Architecture Design Record - Event publishing authorization

## Problem

Elos is designed to publish events from elos-clients or scanner-plugins to
interested subscribers. How to ensure only authorized client can publish
certain events?


## Goals

* avoid exploitation of the event logging system to trigger certain system
  behavior by unprivileged access.
  * i.e. Provides a wrong system view by injecting events
* trace and/or detect possible intrusion

## Assumptions

* Tcp/Ip is required as API-Interface
* Unix Domain Sockets are possible as further API-Interface (i.e. earlier
  available as Tcp/Ip when it comes to booting things up)
* remote access to elosd API is required in future
* a third party authentication instance is possible, identity service
* incoming events via internal scanner-plugin-API are treated as trust-able.
  * Rationale: scanners are loaded as `*.so` files into the elosd process.
	Hence they have full access to any data of the process and further
	protection is pointless.

## Excluded

1. How to ensure only authorized subscribers can subscribe to certain events?
  *  This ADR does not cover handling of sensitive data that need a
	 confidentiality protection (i.e. keys in coredumps, syslog entries with
	 security sensitive data). This aspect is left to another ADR.
  * in this ADR we assume that any one can read every event
2. Transport Layer Security
  * communication can be plain text
  * this ADR consider no efforts in encryption of communication channels
  * no authentication of subscribers
3. Remote event transfer
  * in this ADR it is assumed that elos handles only local events. No event is
	crossing the network wires. This would be part of another ADR.

## Solution

For elos we divide the publishing clients in "privileged" and "normal" clients.
Each event has a classification field which can be used by the integrator to mark critical
events. ( Hint: The idea is to mark critical events as such by the publisher and expect
the subscriber to subscribe or at least to check for the appropriate
classification flags in an event.)
Elosd gets configured to blacklist all events matching a certain event filter
to drop all attempts of publishing events marked as critical from an non secured
channel.

So if a client tries to publish a critical message elos will only dispatch it
if the channel is secured. Subscribers can rely on the fact that events with
security classification flags are only published from privileged and authorized
clients.



### Privileged Client

Privileged client must be authorized and hence needs some authentication. To
permit publishing of black listed events a client must be authorized and
therefore the communication is treated as secured.

On a secured communication it is allowed to send any event, no blackList will be applied.
In a later development stage it could be allowed to send only configured events. This
could be done by RPN-filter rules per client like AccessControlList for events.

To assume a secured communication a client must be authenticated. Currently the
following three approaches are thinkable:
A) Trusted channel: authenticate the local client process connected via
  UnixDomain socket / localhost tcp/ip ( only system local)
B) Certificate: authentication via public private keys (allow remote systems)
C) Token: A third party (identification service) takes over the authentication
  and authorization (allow remote systems)

#### Trusted channel

Trusted channel is the working title for a authentication / authorization
method, that tries to identify the connecting process. This approach only works
on local system as it assumes integrity of the channel and reliable detection of endpoints.
The goal is to gather the following set of information about the connecting process:

* UID -> effective user id of the connecting client
* GID -> effective group id of the connecting client
* PID -> current process id of the connecting client
  * determined at runtime during connecting and is necessary to identify the process in procfs
  * with `/proc/${PID}/*` -> many additional information about the connecting
	process which can be used to authorize the connecting process. In example
	the  name of the process or the link to the original executable file.

The method to obtain the necessary process information differs for the two
considered interface types Tcp/IP and Unix domain sockets.

##### Unix-sockets

For STREAM Unix socket bound to a path name we can use the following options for authorization:

* The Linux sock_diag module via netlink API to query UID,GID and PID. With the
  `getsockopt` call this can be simplified to `getsockopt(clientSocket,
  SOCK_STREAM, SO_PEERCRED, &creds, &creds_length);`.
  * Unfortunately this is very Linux specific, but until nothing other is
	specified we can use it by enabling `_GNU_SOURCE` for this part.
  * The Unix socket must be bound to a path, for unnamed or abstract Unix
	socket this setup will probably not work.


##### local Tcp/ip (loopback interface)

To authorize local Tcp/Ip connections we need also to identify the connecting process. 

This requires:

* root privileges for elos or a process to make the actual lookup for elos
* The peer address must be bound to a local interface (127.0.0.1) , in a later
  stage also the public interface could be allowed if necessary

The actual process lookup is can be done by either calling `netstat` or `ss` or
any equivalent tool. But preferably the kernel interface used by tools like
`netstat` shall be used directly. See therefore `/proc/net` and `sock_diag`.

##### Final step

Now it is possible to match the process information against a set of rule set defined in the configuration:

```json
"trustedProcesses" : {
  "<client name | identifier>": {
    "uid": 0,
    "gid":0 ,
    "executable": "/bin/crinit"
  }
}
```
Or by reuseing RPN-Filter:

```json
"trustedProcesses" : {
  "<nameOfExecutable>": [
    ".uid 0 EQ .exec '/bin/crinit' STRCMP AND",
    ".uid 0 EQ .gid 0 EQ AND"
  ]
}
```

and decide if the client can be treated as privileged.

##### Conclusion

*pro:*
* Can be done without changing the protocol or adding any overhead to it
*cons:*
* the lookup could be time consuming
  * Rational: but usually only one lookup per connection should be feasable
* works only for local processes

#### authorization via certificates

To authorize a connecting client it shall be sufficient to know that the client
can prove its identity. This can be done by using certificates.
Common certificate based socket communication protocols focuses on the tcp/ip
interface, but it shall be possible to use them also via unix domain sockets.

The authorized entity is the process which have access to the certificate and
key. Thus a successful handshake and established  secure connection proves that the
client is authorized to publish classified events. Certificates usually
authenticate a certain domain name, this can be used to distinguish between
different clients.
The configuration for cetificate based authorization would be based on domain
names and could look like:

```json
"trustedProcesses": {
  "com.emlix.asecureapp":{
    "cert": "/etc/elos/client_certs/asecureapp.crt"
  },
  "com.elektrobit.an.evenmore.secureapp":{
    "cert": "/etc/elos/client_certs/amoresecureapp.crt"
  }
}
```

##### how to authorize a publisher

Usualy certificate based authorization comes with encrypted connections and a special connection.
This requires a second socket, where elos expects to create a
secured connection. The process of authorization shall be as follows:

1. client connect to secure Tcp/Ip or Unix socket i.e 128.0.0.1:5432 instead of 127.0.0.1:54321 or /run/elosd.**s**sock instead of /run/elosd.sock
2. elosd accepts incoming connection and initiate handshake (https://wiki.openssl.org/index.php/Simple_TLS_Server)
3a. if successful flag connection as secured and trustable and if needed store domain name
3b. if unsuccessful drop the connection and issue an appropriate event
4. start communication as usual but be sure to use ssl_read/ssl_write methods
5. continue until connection is closed

##### Summary

*pro:*
* with TLS this uses commonly used technology, proven in use
* less complicated to implement and to prove security then secure channel approach
* allows to authenticate and authorize remote clients

*cons:*
* needs two sockets (secure and unsecure)
* needs rework of current connection handling in elos
* due to the encryption a overhead can be expected in for event publishing as for connecting to elosd
* probably increased start time due to loading of certificates
* needs certificate and private key infrastructure.
  * How to issue them
  * How to revoke them
  * How to deploy them
  * How to store keys securely

#### authorization via token

Regardless of the actual choosen implementation of a token based authorization
it would work the following way. The token based approach extends the elos
protocol by an additional field containg a token, which was created by a third
party service. The client previously proves its authentiy against some identity
service and got the token in exchange. The identity service and elosd shares
some common secret. This way elosd can decode the token and verify the client
has authorized itself on some identification service.

The token can contain additonal information see JWT (JSON WebToken JWT.io) for example.

Elosd needs to manage at least one key to verify the tokens.
A configuration could look like:
```json
"eventAuthKey" : "/path/to|or directly the key"
```

##### Summary

*pro:*
* implementations like JWT are proven in use
* allows to authenticate and authorize remote clients
* allows to store costom additional information in a token

*cons:*
* elos protocol must be changed
* needs rework of current connection handling in elos
* overhead of token length in each elosd paket
* needs additional infrastructure.
  * identity service
  * How to revoke tokens
  * How to store keys securely

### Normal Clients (current status)

Normal clients doesn't need any authentication or authorization. All
connections of normal clients are treated as non-secure and hence need
filtering of black listed events.


### Configurable elements

To manage privileged an normal clients two configuration items are necessary:

* *for secure clients:* (Certs, Token) or a tuple of (UID, GID, ELF-Path)
```json
"trustedProcesses" : {
  "<client name | identifier>": {
    "uid": 0,
    "gid":0 ,
    "executable": "/bin/crinit"
    "cert": "/etc/elos/client_certs/..."
    "token": "yxzkjlsdj87238..."
  }
}
```

* *for normal clients:* a BlackList defined as RPN-Filter i.e. blacklist all as 'Security' classified events:
```json
"EventBlacklist" : ".event.classification 0x4 EQ"
```


### The blacklist filtering

For the blacklist filtering approach there are to variants possible, when to
check for a privileged clients.

1) check on establishing of connection

If the client connection is authorized, the blacklist check can be skipped. If
the client is not authorized (normal client) a black list check must be
performed on each incoming event. If the client is not authorized and the event
hits the blacklist, the event is dropped.

2) check on receive of blacklisted event

Each event is checked if matches the blacklist and if the blacklist matches it
is checked if the client is privileged, otherwise the event is dropped.

The attempt of sending a classified event from a normal client will create a
new event tirggered by elos it self to broadcast the insident:
```json
{
  "date": [42,0],
  "source": {
    "appName": "elosd",
    "pid": 1234
  },
  "severity": 3,
  "hardwareid": "<hardwareid>",
  "classification": 324,
  "messageCode": 8007
}
```

The value `classification` value is a concatenation of the flags `elos|Security|IPC`.

## Decision:

In both cases we have to match all incoming events from normal clients (not
secure) against the blacklist. So the first solution gives us a little
advantage in processing speed by skipping the blacklist check entirely for
privileged clients (secure).

Thus we choose option 1 to implement the blacklist check. For client
authentication approach *A* of privilieged clients is choosen.
