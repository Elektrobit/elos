# Design Decisions - systemd Journal Integration

## Problem

systemd-based environments typically use `systemd-journald` to collect logging data from various sources in the system and store this data in structured files ("journals").
Here, the role of `systemd-journald` partially overlaps with traditional syslog daemons and also elos.

`systemd-journald` requires exclusive access to `/dev/log`, which collides with elos because the elos syslog scanner by default listens on this socket to receive syslog messages.

To use syslog daemons and, thus, elos in the role of a syslog daemon, two options exist:
 - let the syslog daemon read the journals and watch for changes
 - let `systemd-journald` forward its data to the syslog daemon over a UNIX domain socket using the `syslog` protocol

The man page of `journald.conf` states this in more detail:
```
FORWARDING TO TRADITIONAL SYSLOG DAEMONS
       Journal events can be transferred to a different logging daemon in two different ways. With the first method, messages are immediately forwarded to a socket (/run/systemd/journal/syslog), where the
       traditional syslog daemon can read them. This method is controlled by the ForwardToSyslog= option. With a second method, a syslog daemon behaves like a normal journal client, and reads messages from the
       journal files, similarly to journalctl(1). With this, messages do not have to be read immediately, which allows a logging daemon which is only started late in boot to access all messages since the start
       of the system. In addition, full structured meta-data is available to it. This method of course is available only if the messages are stored in a journal file at all. So it will not work if Storage=none
       is set. It should be noted that usually the second method is used by syslog daemons, so the Storage= option, and not the ForwardToSyslog= option, is relevant for them.
```

## Influencing factors

The following constraints have to be taken into account for a suitable solution:
* storage: `systemd-journald` may be configured to not store journal files in the filesystem at all. Conversely, elos may store its events to one of several storage backends. It may be desirable to store events only in one of these locations.
* boot order: there should be a defined point in the boot process at which elos is guaranteed to be ready.
* compatibility: the solution should place as few restrictions on the system architecture as possible.

## Assumptions

The following assumptions have been made in the decision process:
* apart from elosd, no other syslog daemon is used in the target system.

## Considered Alternatives

### 1) elos as a journal client

elos accesses the journal files using the `sd_journal` API.

*pros*
* easy access to journal entries written before the start of elosd
* provides all data about the recorded events that is available in journal fields (see the man page `systemd.journal-fields`)
* allows co-existing with other syslog daemons

*cons*
* requires journal files to be available (`Storage=none` in `journald.conf` is not an option.)
* requires journal files to always be stored in the filesystem. This will lead to duplicate event storage if Elos is also configured to log events to files.

### 2) Message forwarding to elos (syslog protocol)

`systemd-journald` supports forwarding messages to a syslog daemon over a UNIX domain socket in the syslog format.
With this option, elos takes the role of such a syslog daemon.

The [freedesktop documentation](https://www.freedesktop.org/wiki/Software/systemd/syslog/) gives a detailed explanation about the integration of syslog daemons in a systemd environment. In short, this means for elos:

Instead of directly using `/dev/log`, elos receives forwarded syslog messages from `systemd-journald` on the socket `/run/systemd/journal/syslog` instead. By systemd convention, it does not bind to this socket on its own. Instead, the socked opened by systemd and passed to elos using the [socket activation mechanism](https://www.freedesktop.org/software/systemd/man/latest/systemd.socket.html).
By default, systemd starts the elos service on-demand (i.e. as soon as `systemd-journald` first tries to forward data to the socket).

However, by setting up the dependencies and ordering in `elos.service` appropriately, elos can be started before `systemd-journald` and independent of actual socket accesses.

*pros*
* minimal effort in elos: implement socket passing to the syslog scanner (receive opened socket using the `sd_listen_fds` API instead of creating and opening it)
* events are directly passed by socket with no need for (intermediate) journal files
* does not require journal files
* provides all currently-required event data
* quickest to implement

*cons*
* not all available journal fields are forwarded using this protocol
** But: the extra data is currently not required by elos

### 3) Message forwarding to elos (Journal Export Format)

Starting with version 256, `systemd-journald` has the option of forwarding messages to a UNIX domain socket in the systemd-native [Journal Export Format](https://github.com/systemd/systemd/blob/main/docs/JOURNAL_EXPORT_FORMATS.md) by
setting the `ForwardToSocket` option.

*pros*
* journal storage can be deactivated
* provides all data about the recorded events that is available in journal fields (see the man page `systemd.journal-fields`)
* allows co-existing with other syslog daemons

*cons*
* requires systemd 256 or later, which may not be available in all targets yet (specifically, the Yocto Scarthgap (5.0) release uses systemd 255)
* larger development effort compared to the syslog protocol
* extra data fields (compared to option 2) are currently not required by elos

## Decision

After checking for completeness of alternatives and taking all pros and cons into account, the following decision was taken:

**2**
