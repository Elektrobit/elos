# elos Init System Integration

This directory contains example integrations for systemd and sysvinit. These are usable as-is or adapted to the desired use case.

## sysvinit

The provided sysvinit init script starts and stops elosd. It is installed if the CMake option `INSTALL_ELOSD_SYSVINIT_SCRIPT` is enabled  (default: disabled).

## systemd

The systemd service unit (`elosd.service`) starts and stops elosd as a syslog daemon which expects the `syslog.socket` to be passed in by systemd.
This requires the `systemd-journald` option `ForwardToSyslog` to be set - this makes `systemd-journald` forward all live journal data to elosd using this socket.

The unit is configured to explicitly start elosd before systemd-journald. Alternatively, elosd can be started on-demand by systemd once the `syslog.socket` is accessed;
to achieve this, remove all explicit dependency and ordering relations apart from `Requires=syslog.socket`.

The service unit is installed if the CMake option `INSTALL_ELOSD_SYSTEMD_SERVICE` is enabled (default: enabled if elosd is configured with systemd support by setting `ELOSD_SYSTEM=1`).
