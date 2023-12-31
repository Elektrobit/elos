# Architecture Design Record - Detecting Coredumps

## Problem
Kernels core dump will write a process image into processes current working directory by default. While writing the process image it does not save additional information like executable pathname, UID,GID, Number of signal causing dump, etc. . So the problem here is to check if a coredump has happened, if it has then, retrieve the coredump data and store it.


## Influencing factors

## Assumptions
The assumption is that kernel coredump pattern is accessible via the proc interface.

## Considered Alternatives

### Add a small executable to forward kernel coredump images to elosd.

To setup an own location where the dumps will be written and to add a chance to get the additional information kernels core dump provides via specifiers, a small binary that forwards this information to the elos daemon shall be created. The binary will then be run with root permission and the core dump is given as standard input.

#### Prepare kernel coredump pattern

The kernels coredump pattern can be set to pipe the resulting coredump
image to a user-space binary including some specifiers that will be
replaced by the kernel during binary invocation.

To setup the pattern automatically, **as root** call the *coredump* binary
without any parameters. This will setup the following coredump pattern
in */proc/sys/kernel/core_pattern*:
``` shell
|<PATH_TO_COREDUMP_BINARY>/coredump %P %E %u %g %s %t %h
```

See manpage (*man 5 core*) for more details about core pattern and the
different specifiers used here.

#### Trigger a logging event and coredump image write

After preparing kernel coredump pattern ensure the elosd daemon is
running. Then let an application of your choice run into a
segmentation fault to trigger kernels coredump handling.

The elosd logfile should then contain a logline that starts with
```
{"messageCode":5005, ...
```
and the coredump image will be saved under */tmp* directory as
core.\<timestamp\>.\<PID\> .


#### Manually trigger a logging event and coredump image write

To manually test *coredump* binaries behaviour without invoking
kernels real coredump functionality one could try with the
following steps:

* start logdaemon elosd
* manually invoke coredump binary with some valid parameters, e.g.:
``` shell
$ echo "THIS IS THE DUMP" | /usr/local/bin/coredump 1 /usr/bin/somebinary 2 3 11 333333 myhostname
```
* then the file content of */tmp/core.333333.1* should contain the line "THIS IS THE DUMP"
* the elosd logfile should contain the new event line:
```
{"topicId":42,"topic":"COREDUMP_STARTED","date":[333333,0],"source":{"appName":"\/usr\/bin\/somebinary","fileName":"\/usr\/bin\/somebinary","pid":1},"severity":1,"hardwareid":"myhostname","classification":512,"messageCode":5005,"payload":"core dumped to \/tmp\/core.333333.1, signal=11, UID=2, GID=3"}
```

## Decision

### Rationale

### Open Points

Usage of elos-coredump in a container is not recommended presently, because elos-coredump uses the linux kernel interface ```/proc/sys/kernel/core_pattern``` to pipe coredump events into elosd. This
strategy works when used in a target system or an emulator, since the kernel interface is that of the target or emulator. In a container however the kernel interface is that of the host system on which the container is running. This shared interface leads to a difference in the creation of the coredump files based on how the core pattern set. When the the core pattern is an simple path to a files
like ```/tmp/core.%t.%P```, when a segmentation fault occurs, the coredump file is created locally in the container file system and written to. This is not the case when the core pattern has coredump piped into an executable like ```|/usr/bin/elos-coredump %P %E %u %g %s %t %h```, the coredump files are created in the host file system and not in the container. When the executble to which the coredump is piped is not present in the host, then the coredump file is not created. Additionally when the coredump file is created, it is in the host system, and is not not available in the container unless the file's directory is shared with the container. Taking this behaviour into considertaion, elos-coredump shall be avoided in a container.
