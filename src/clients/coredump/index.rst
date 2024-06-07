Coredump Client
===============

The coredump client converts a coredump that has occurred into an elos event. The coredump properties are set through the coredump client configuration file `coredump.json` file.


.. literalinclude:: /src/clients/coredump/coredump.json
   :language: json


The configuration consists of the following parameters:

-  `totalmem` - The absolute value of memory available for the purpose of coredump files in a give path. To calculate the size of free memory available in a given path the `statvfs` standard `C` library function can be used. See manpage (* man 3 statvfs) for more information.
-  `maxcoredump_size` - The maximum value which a single coredump file can be.
-  `totalmem_rel` - The memory available for the coredump files relative to the free memory available in the directory where the files shall be stored.
-  `maxcoredump_size_rel` - The memory available for a single coredump file relative to the free memory available in the directory where the files are stored.
-  `maxcoredump_cnt` - The maximum number of coredumps allowed.
-  `coredumppath` - Path were the coredumps files shall be stored.
-  `coredump_per_exe` - The maximum number of coredumps that can be attributed to a single process.

The configuration file will be installed at the `etc` directory during build. When the build is successful, the client can be used as given below.


Prepare kernel coredump pattern
-------------------------------


The kernels coredump pattern can be set to pipe the resulting coredump image to a user-space binary including some specifiers that will be replaced by the kernel during binary invocation.

To setup the pattern automatically, **as root** call the *coredump* binary without any parameters. This will setup the following coredump pattern in */proc/sys/kernel/core_pattern*:


`<PATH_TO_COREDUMP_BINARY>/coredump %P %E %u %g %s %t %h`


See manpage (*man 5 core*) for more details about core pattern and the different specifiers used here.


Trigger a logging event and coredump image write
------------------------------------------------


After preparing kernel coredump pattern ensure the elosd daemon is running. Then let an application of your choice run into a segmentation fault to trigger kernels coredump handling.

The elosd logfile should then contain a logline that starts with `{"messageCode":5005, ...` and the coredump image will be saved under */tmp* directory as core.\<timestamp\>.\<PID\> .


Manually trigger a logging event and coredump image write
---------------------------------------------------------

To manually test *coredump* binaries behaviour without invoking kernels real coredump functionality one could try with the following steps:

- start log daemon elosd
- manually invoke coredump binary with some valid parameters, e.g.:

 `$ echo "THIS IS THE DUMP" | /usr/local/bin/elos-coredump 1 /usr/bin/somebinary 2 3 11 333333 myhostname`

- then the file content of */tmp/core.333333.1* should contain the line "THIS IS THE DUMP"
- the elosd logfile should contain the new event line `{"topicId":42,"topic":"COREDUMP_STARTED","date":[333333,0],"source":{"appName":"\/usr\/bin\/somebinary","fileName":"\/usr\/bin\/somebinary","pid":1},"severity":1,"hardwareid":"myhostname","classification":512,"messageCode":5005,"payload":"core dumped to \/tmp\/core.333333.1, signal=11, UID=2, GID=3"}`
