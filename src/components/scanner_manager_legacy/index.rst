Scanner Manager
===============

Elosd allows to observe the system for certain events and publish them
directly via the EventProcessor. The actual observation and publishing
is done within the Scanners. The Scanner Manager has the task to
maintain them.

-  A Scanner:

   -  Is a shared object that provides the Scanner-API by exporting the
      function symbols defined in scanner.h
   -  Intended to publish messages for arbitrary events
   -  Are not explicitly limited in number, but by system resources like
      open file descriptors
   -  Is expected to achieve lower latency then publishing messages
      through tcp/ip

-  Scanner Manager:

   -  Locate shared object files that shall be loaded as Scanner
   -  Instantiate, configure and run a Scanner in a separated thread
   -  Stop and restart scanner if necessary
