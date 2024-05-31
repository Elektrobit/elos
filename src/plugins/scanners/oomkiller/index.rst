Oom killer invoked event scanner
================================

This scanner runs a subscription filter every `10ms` on kernel message events via a regex filter that filters
the `kmsg` events for `oom killer invocation`. When an event matches the scanner then the scanner publishes a
new event with `messageCode 5020`, `appName` the application terminated and its corresponding `pid`.
