# Architecture Design Record - libelos logger concept

## Problem


The libelos\_logger currently connects to elosd using the TCP/IP connection. Given that elosd will in future support 
UNIX domain sockets and shared memory for interaction with clients, the libelos\_logger shall also support the same. 


```{uml}
node Hypervisor {
     node System-with-Elosd {
        component Elosd as ED {
            rectangle config [
              TCP/IP
              UNIX
              SHMEM
            ]
        }
        component libelos as LE_native {
            rectangle config [
                Any config
            ]
        }
        component SafetyApp as ATIL
     }

     node SafetySystem {
        component libelos as LE_shmem {
            rectangle shmconfig [
               SHMEM config
            ]
        }
        component SafetyApp as SA_SHMEM
     }

     node SystemTcpIP {
        component libelos as LE_tcp {
            rectangle tcpconfig [
                TCP/IP config
            ]
        }
        component SafetyApp as SA_TCP
     }
}

ATIL --> LE_native : elosLog()
LE_native --> ED : elosLog() via config
SA_SHMEM --> LE_shmem : elosLog() 
LE_shmem --> ED: elosLog() via SHMEM
SA_TCP --> LE_tcp : elosLog() 
LE_tcp --> ED: elosLog() via TCP/IP
```


The main part of this problem is the implementation of switching the connection between elosd and the logger based on how 
elosd is configured. 


1) **Elosd Configuration** 


   Will elosd be always configured with support for all types of connections? Currently connection to elosd is configured 
   using the Plugins in the `config.json` 

   ```json
   
   "Plugins": {
       "LocalTcpClient": {
           "File": "client_dummy.so",
           "Run": "always",
           "Config": {
               "Port": 54321,
               "Interface": "127.0.0.1",
               "EventBlacklist": ".event.messageCode 2000 EQ",
               "authorizedProcesses": [
                   ".process.uid 0 EQ .process.gid 0 EQ .process.exec '/bin/elosc' STRCMP AND",
                   ".process.gid 200 EQ .process.exec '/bin/elosc' STRCMP AND",
                   ".process.pid 1 EQ"
               ]
           }
       }
   }
   
   ``` 


   The plan is to implement similar Plugins for UNIX domain sockets and shared memory. So will all the connection options be
   available for the client to connect to always?
 

2) **Client Configuration**


   When implementing a client for elos, the client uses the `elosConnect` function to connect with elosd. The logger then
   creates a separate connection to elosd to publish log events. Both the application as well as the logger should be able to
   connect with elosd using separate connection instances as well as via a single connection instance.
  
 
3) **Logger Configuration**


   When a logger is created for the client, a single logger instance is created and it establishes a separate connection to 
   elosd to publish log events. The feature that is not present here is that :

   - a client can have multiple logger instances.
   - any client logger instance should be able to connect separately with elosd as well hook onto an established connection with elosd
     by the client


4) **Libelos API**


   The `libelos` library supports only connection with elosd via TCP/IP. This should be expanded to include connection support for 
   Unix domain sockets and shared memory objects.
   


## Influencing factors


* Event logger API should wrap the connection based switching to elosd so that, the logging
  function remains the same across the clients with different connection types.
* When `libelos` support for connection based on linux shared memory is added then read write synchronization shall be handled using
  spinlocks as discussed in [shmem_locking](shmem_locking.md)
* Event Logger should be thread safe


## Considered Alternatives

In order to solve the connection problem between `elosd`, client and logger, the following changes are necessary:


1) **ELosd Configuration**


   Elosd shall support communication with client using unix domain sockets and shared memory objects (POSIX-IPC). Multiple connection
   interfaces shall be configured according to user requirements. An example configuration is given below:


   ```json
   
   "Plugins": {
       "LocalTcpClient": {
           "File": "client_dummy.so",
           "Run": "always",
           "Config": {
               "Port": 54321,
               "Interface": "127.0.0.1",
               "EventBlacklist": ".event.messageCode 2000 EQ",
               "authorizedProcesses": [
                   ".process.uid 0 EQ .process.gid 0 EQ .process.exec '/bin/elosc' STRCMP AND",
                   ".process.gid 200 EQ .process.exec '/bin/elosc' STRCMP AND",
                   ".process.pid 1 EQ"
               ]
           }
       },
       "LocalUnixClient": {
           "File": "unix_dummy.so",
           "Run": "always",
           "Config": {
               "Path": "/tmp/elos_unix_socket"
           }
       },
       "LocalShmemClient": {
           "File": "shmem_dummy.so",
           "Run": "always",
           "Config": {
               "Name": "/elos_shmem",
               "Size": 1024
           }
       }
   }
   
   ```   

2) **Client Configuration**

    A client shall be configured with the following :
    
    - a connection instance populated with connection parameters.
    - termination of connection instance.

    The client should be able to connect to elosd based on the its connection requirements. In order to connect, one of
    the `libelos` API functions can be used. The connection instance shall be disconnected after use. A client should also be
    able to instantiate more than one connection instance. A simple example of a client is given below:

   ```C
      int main() {
        elosSession_t *session;
        elosConnectTcpip("127.0.0.1", 1234, &session);
        elosDisconnect(session);
     	return 0;
      }
   ```

3) **Logger Configuration**


   In order to configure a logger, first an logger object is created. In order to initialize this object an `elosInitLogger`
   function is added to the `libelos_logger` library.
   
   ```C
   typedef struct elosLogger {
      elosSession_t *session;
   } elosLogger_t;

   safuResultE_t elosInitLogger(NULL, elosSession_t *session);
   safuResultE_t elosDeleteLogger(NULL, elosSession_t *session);
   ```

   The initialize function which hooks the logger to a connection instance. When this hook is established, the logger can communicate 
   with elosd using the connection instance. If the logger parameter is `NULL` in `elosInitLogger` function then a default logger is 
   initialized with the given session instance, else the given logger object is initialized.

   A code snippet for default logger initialization is given below :
  
      
   ```C
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosSession_t *session;

    result = elosConnectTcpip("127.0.0.1", 54321, &session);
    if (result == SAFU_RESULT_OK) {
        result = elosInitLogger(NULL, session);
    }

    elosDisconnect(session);
    elosDeleteLogger();
    return 0;
   ```


    AN example for one ore more logger initialization one ore more connection instance is geiven below :


   ```C
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosSession_t *sessiontcp;
    elosSession_t *sessionunix;
    elosLogger_t *loggerunix;
    const char *path = "/tmp/elos_unix_socket"

    result = elosConnectTcpip("127.0.0.1", 54321, &sessiontcp);
    if (result == SAFU_RESULT_OK) {
        result = elosInitLogger(NULL, sessiontcp);
    }
    
    result = elosConnectUnix(path, &sessionunix);
    if (result == SAFU_RESULT_OK) {
        result = elosInitLogger(loggerunix, sessionunix);
    }

    elosDisconnect(sessiontcp);
    elosDisconnect(sessionunix);
    elosDeleteLogger(NULL);
    elosDeleteLogger(loggerunix);
    return 0;
   ```
    
4) **Libelos API**


   The existing libelos API functions should be edited in order to provide support for different connection types. The functions that
   are needed are :

   ```C
   elosConnectTcpip(const char *host, uint16_t port, elosSession_t **session)
   elosConnectUnix(const char *path, elosSession_t **session)
   elosConnectShmem(const char *name, elosSession_t **session)
   ```
   
   Presently the libelos is not configurable since it had only one connection option. But when the above functions are added, the user
   should be able to configure libelos according to the available connection types. This configuration shall be implemented using 
   compile time flags.

   As part of the `libelos` library, additional logger functions are needed in the `libelos_logger` library as well, these include 

   ```C
   elosInitLogger(elosLogger_t *logger, elosSession_t *session);
   ```
 
   The above function initializes the logger object. If the no object is provided i.e. `logger` is `NULL` then a default
   logger is initialized with the connection object provided.


   ```C
   elosDeleteLogger(elosLogger_t *logger);
   ```

   The above function deletes an initialized logger object. When a default logger is initialized then calling this function
   deletes the default function. When N number of loggers are initialized make sure to delete all of them   

   ```C
   elosSetLogger(elosLogger_t *logger);
   ```

   When multiple loggers are initialized with different connections, this function helps to set the logger that shall be used
   to log all forthcoming log events.


## Solution



1) Elosd shall be configured according however client wishes, this should not have any bearing on the client logger.


2) The client shall implement how it will be connected to elosd. The client can initialize a logger an use its event logging
   capabilities.


3) The logger shall be initialized by the client. A logger can be instantiated separately or the default logger can be used.
   Similarly a logger can have a separate connection instance to elosd or can hook to the client's connection instance.


3) The `libelos` library shall provide functions for supporting connection to elos via TCP/IP, unix domain sockets and shared memory
   objects (POSIX-IPC). The functions are configurable using a compile time flags. Based on the `libelos` configuration the client
   connection set up can be implemented.
