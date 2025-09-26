DLT HV - Scanner
================

Reads the provided DLT HV log ring buffer and publishes all events as they come in.

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
         "DeviceFile": "/dev/shm/test_dlt_shmem",
         "OffsetAddress": "0x1000",
         "BufferSize": 1448,
         "ScanFrequency": 2277,
         "HardwareId": "The machine publishing DLT",
         "Pid": 3,
         "AppId": "The DLT aplication"
      }
   }


- ``DeviceFile``: The (memory mapped) file to open for the DLT ring buffer.
- ``OffsetAddress``: The offset address where the buffer starts in the file (default if not provided is 0)
  Has to be a multiple of the page size of the system its running on.
  Can be a string encoding the address or a number
- ``BufferSize``: The memory size of the buffer.
  This is only used to check that the size stated in the buffer header is correct.
  If a size of 0 is given the check is skiped, this is the default if nothing is configured.
  Can be a string encoding the size or a number.
- ``ScanFrequency``: The Frequency (in hz) with wich the scanner should wake up and check for new log entries.
  Default is 2277 hz
- ``HardwareId``: An identifier for the machine publishing the DLT events. Default is the content of "/etc/machine-id".
- ``Pid``: The pid of the proccess publishing the DLT events. Default is 0 wich disables it.
- ``AppId``: An identifier for the proccess publishing the DLT events.
   Default is the <DltHvScanner Instance name>.


Configuration structure
~~~~~~~~~~~~~~~~~~~~~~~

.. code:: bash

   DltHvScanner
   ├── File
   ├── Run
   └── Config
       ├── DeviceFile
       ├── OffsetAddress
       ├── BufferSize
       ├── HardwareId
       ├── Pid
       └── AppId

