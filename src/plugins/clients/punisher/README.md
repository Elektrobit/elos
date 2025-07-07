# Punisher

A simple event handling plugin for elos written in C++.
It reacts to events send by nightwatch.

## Thresholds

This plugin is used in combination with the scanner plugin nightwatch.
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

To run the plugin with elos make sure the following entry is present 
in the elosd configuration under root/elos/Scanner/Plugins:

```json
"Punisher": {
    "File": "client_punisher.so",
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

```bash
cmake -B build .
make -C build install
```
