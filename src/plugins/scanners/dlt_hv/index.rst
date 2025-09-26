DLT HV - Scanner
================

Reads the provided DLT HV log ring buffer and publishes every event.

Configuration
-------------

Json
~~~~

Under `root/elos/Scanner/Plugins` add:

.. code:: json

   "<DltHvScanner Instance name>": {
      "File": "scanner_dlt_hv.so",
      "Run": "always",
      "Config": {
         "DeviceFile": "<shared memory file name>"
      }
   }

- ``DeviceFile``: The shared memory file in to open for the DLT ring buffer.
  Only the file name has to be provided not the full path.

Configuration structure
~~~~~~~~~~~~~~~~~~~~~~~

.. code:: bash

   DltHvScanner
   ├── File
   ├── Run
   └── Config
       └── DeviceFile

