shmem
=====

The shmem scanner is a proof of concept for converting eb log entries into elos events over a shared buffer data structure.

Configuration
--------------

The scanner is configured via the elosd.json file, with a block like the following located under ``root/elos/Scanner``::

    "Shmem": {
        "ShmemFile": "scanner_shmem",
        "ShmemCreate": true,
        "ShmemLogEntries": 256,
        "ShmemOffset": 0,
        "SemFile": "scanner_shmem_sem",
        "SemCreate": true
    }

::

Note that all options are currently mandatory, leaving options out will cause the shmem scanner to abort with errors during initialization.

* *ShmemFile*, string: Filename of the shared memory. Due to the way ``shm_open()`` works an absolute path can't be specified, the file will be placed under ``/dev/shm/<ShmemFile>`` instead.
* *ShmemCreate*, boolean: Creates and initializes the shared memory when set to ``true``. Will open an existing shared memory when set to ``false``.
* *ShmemLogEntries*, int: How many eb logging entries the ringbuffer can hold before overflowing.
* *ShmemOffset*, int: Offset within the shared memory region at which the eb log ringbuffer data resides.
* *SemFile*, string: FIlename of the shared semaphore. Same behaviour as with ShmemFile, with the semaphore being created as ``/dev/shm/<SemFile>``.
* *SemCreate*, boolean: Create the semaphore when set to ``true``, open it when set to ``false``

*Attention*: This is a proof of concept, so implementation details as well as the configurationn options may change.

:doc:`../index.rst`
