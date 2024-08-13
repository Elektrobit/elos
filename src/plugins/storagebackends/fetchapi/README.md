# A plugin for the elos fetchapi

A super simple in memory backend plugin for elos written in C++.
All events are stored in a ring buffer of a number of events specified by the configuration.

## Configuration

In the elosd configuration under `root/elos/EventLogging/Plugins` add:

```json
"fetchapi": {
    "File": "backend_fetchapi.so",
    "Run": "always",
    "Filter": [
        "1 1 EQ"
    ],
    "Config": {
        "BufferSize": 1000
    }
}
```

- `BufferSize`: the number of events to store in the ring buffer.
    This is not the total amount of memory used for storing the events.

