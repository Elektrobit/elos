# Nightwatch

A simple monitor plugin for elos written in C++.
It creates events when overloaded applications were found.

## Thresholds

This plugin is used in combination with the client plugin punisher.
Nightwatch generates events based on thresholds.

When no threshold defined by the user is reached, 
nightwatch will inform the system via event.

When threshold 1 defined by the user is reached, 
nightwatch will inform the system via event.

When threshold 2 defined by the user is reached, 
nightwatch will inform the system via event.
Punisher collects the event and restarts the application named in the event.

When threshold 3 defined by the user is reached, 
nightwatch will inform the system via event.
Punisher collects the event and kills the application named in the event.

## Configuration

The plugin can be configured to set applications for
monitoring, thresholds and test intervals.
The structure of the config file looks as follows:

```yaml
systemmonitoring:
# Overwrites default values if set here
    testinterval: ... # default is 600 needed in miliseconds (optional)
    memthreshold1: ... # default is 1.0 (optional)
    memthreshold2: ... # default is 1.5 (optional)
    memthreshold3: ... # default is 2.0 (optional)
    cputhreshold1: ... # default is 1.0 (optional)
    cputhreshold2: ... # default is 1.5 (optional)
    cputhreshold3: ... # default is 2.0 (optional)
    monitoring:
        monitoringname1:
            testinterval: ... # default is 600 needed in miliseconds (optional)
            memthreshold1: ... # default is 1.0 (optional)
            memthreshold2: ... # default is 1.5 (optional)
            memthreshold3: ... # default is 2.0 (optional)
            cputhreshold1: ... # default is 1.0 (optional)
            cputhreshold2: ... # default is 1.5 (optional)
            cputhreshold3: ... # default is 2.0 (optional)
            applications:
                appname1:
                    maxmemusage: ... # needed in KB
                    maxcpuload: ... # needed in %
                    memthreshold1: ... # default is 1.0 (optional)
                    memthreshold2: ... # default is 1.5 (optional)
                    memthreshold3: ... # default is 2.0 (optional)
                    cputhreshold1: ... # default is 1.0 (optional)
                    cputhreshold2: ... # default is 1.5 (optional)
                    cputhreshold3: ... # default is 2.0 (optional)
                appname2:
                    ...

        monitoringname2:
            ...
```

To run the plugin with elos make sure the following entry is present 
in the elosd configuration under root/elos/Scanner/Plugins:

```json
"Nightwatch": {
    "File": "scanner_nightwatch.so",
    "Run": "always"
}
```

## Building using cmake and make

Make sure to build [elos](https://elektrobit.github.io/elos/doc/userManual.html#elosd-installation-setup) before building the plugin.

Install dependencies:

- libelos
- libelosplugin
- [safu](https://github.com/Elektrobit/safu)
- [samconf](https://github.com/Elektrobit/samconf)
- yaml-cpp

```bash
cmake -B build .
make -C build install
```
