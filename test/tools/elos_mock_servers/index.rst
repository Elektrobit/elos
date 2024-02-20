Elosd Mock Servers
==================

The elos mock servers provide a stand in  for elosd servers to check a 
particular type of connection. As the first implementation a mock server
for connecting to a Unix domain socket is provided. 

The mock servers are provided as means to check clients which use a 
different connection than that provided in elosd (Presently elosd provides 
support only for  TCP/IP connection). The mock server provided here is one
that simulates an Unix domain socket server.

The mock servers unix domain socket is given at ``/tmp/elos_unix_socket``
When a client is connected to this server an tries to publish an elos event,
then server prints the received message to the terminal and the client 
receives a dummy error message as reply.
 
::

  cd <elos>
  ./test/tools/elos_mock_servers/mock_uds.py
