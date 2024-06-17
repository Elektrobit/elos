Kmsg - Scanner
==============

Reads the kernel log ring buffer from /dev/kmsg and publishes depending
on pattern matching ([WIP] currently each line) as a “KMSG” event. The
scanner tries to open the configured file for reading, if it is not
found at the specified location it will try to create a FIFO node using
``mkfifo()``. Multiline kernel messages are not handled right now.

Published events
----------------

-  ``KMSG`` –> each line read from /dev/kmsg is published as it is in
   the event payload field. But also parsed to set severity,
   classification and date of the published event. All events generated
   by the kmsg scanner have the message code 1111 “kernel log message”.
   If the message could not be parsed for some reason severity and
   classification will stay empty and the message code is set to 3422
   “message not understood”.

   -  format is:

   .. code:: json

      {
          "date": [1684239716, 1668901000],
          "source": {
              "fileName": "\/dev\/kmsg"
          },
          "severity": 3,
          "classification": 1,
          "messageCode": 1111,
          "payload": "3,215,264071662,-;squashfs: Unknown parameter 'tmpfs'"
      }

Configuration
-------------

Compile time:
~~~~~~~~~~~~~

-  ``ELOSD_KMSG_FILE`` –> the path where to open expect a character
   device or FIFO file node, default is ``/dev/kmsg``

Environment:
~~~~~~~~~~~~

-  ``ELOS_KMSG_FILE`` –> the path where to open expect a character
   device or FIFO file node, default is ``ELOSD_SYSLOG_PATH``

Json
~~~~

Under `root/elos/Scanner/Plugins` add:

.. code:: json

   "<KmsgScanner Inastance name>": {
      "File": "scanner_kmsg.so",
      "Run": "always",
      "Config": {
         "KmsgFile": "/dev/kmsg"
      }
   }


Configuration structure
~~~~~~~~~~~~~~~~~~~~~~~

.. code:: bash

   KmsgScanner
   ├── File
   ├── Run
   └── Config
       └── KmsgFile

