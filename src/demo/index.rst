Demos
=====

Elos comes with a set of demo applications to show certain aspects of elos.

elosMon – publish events as mails
---------------------------------

.. program-output:: elosMon -h

elosc – simple commandline client
---------------------------------

.. program-output:: elosc -h

To verify the elos version use :

.. command-output:: elosc -v

To subscribe on any event :

.. command-output:: elosc -s "1 1 EQ"
   :returncode: 1

To subscribe on coredump events :

.. command-output:: elosc -s ".event.messageCode 5005 EQ"
   :returncode: 1

To list all events logged :

.. command-output:: elosc -f "1 1 EQ"
   :returncode: 1

To list all logged events that are critical :

.. command-output:: elosc -f "event.severity 1 EQ"
   :returncode: 1

To publish an event :

.. command-output:: elosc -p '{"messageCode": 1001, "severity": 4, "payload": "a custom event with severity INFO (4)"}'
   :returncode: 1

To publish 100 events as fast as possible :

.. command-output:: elosc -c 100 -p '{"messageCode": 1001, "severity": 4, "payload": "publish 100 events as fast as possible"}'
   :returncode: 1

To publish 100 events with 50Hz :

.. command-output:: elosc -c 100 -r 50 -p '{"messageCode": 1001, "severity": 4, "payload": "publish 100 events at 50Hz"}'
   :returncode: 1

demo_libelos_v2 – publish subscrie demo using libelos
-----------------------------------------------------

To verify the elos version use :

.. command-output:: demo_libelos_v2 -v

demo_eventbuffer – test performance of event buffer
-----------------------------------------------------

This demo was manly developed to test and verify the `EventBuffer` component.

.. program-output:: demo_eventbuffer -h

To benchmark the event buffers on a target with default value run:

.. command-output:: demo_eventbuffer

To benchmark the event buffers on a target to find optimal configuration :

.. command-output:: demo_eventbuffer --publisher 100 --consumer 10 --messageCount 1000 --eventLimit 300

syslog_example – demonstrate the and test the syslog scanner
------------------------------------------------------------

With the `syslog_example` demo it is possible to verify if the syslog scanner
is configured correctly. It can also be used to develop the mapping rules for
the syslog scanner.

To verify that the syslog scanner is working:

.. command-output:: syslog_example "Syslog scanner test"; elosc -f ".event.payload 'Syslog scanner test' STRCMP"
   :returncode: 1

Simulate syslog lines and test a filter if it can be used as mapping rule for the syslog-scanner:

.. command-output:: syslog_example "Jun 15 19:56:23 hostnameXY wpa_supplicant[420]: wlp0s20f3: WPA: Group rekeying completed with 00:00:00:00:00:00 [GTK=TKIP]"
   :caption: simulate the syslog line
   :returncode: 1

.. command-output:: elosc -f ".event.payload r'WPA: Group rekeying' REGEX"
   :caption: check if rule mtches the syslog line
   :returncode: 1

tinyElosc – minimized elos client to send raw payload
-----------------------------------------------------

This demo was developed to send raw formatted payload. The payload is not
preprocessed and validated by any JSON parser. This can be used to send
malformed or any arbitrary encoded payload.

.. program-output:: tinyElosc -h

To verify the elos version use :

.. command-output:: tinyElosc -v
   :returncode: 1


Test elos for arbitrary not valid encoded payload:

.. command-output:: tinyElosc -p "<wrong>this is not a JSON string</wrong>"
   :caption: try to publish an XML string instead of a JSON string
   :returncode: 1

elos_log4c_demo - a demo program to show log4c logging in elos
--------------------------------------------------------------

This demo was developed to demonstrate the usage of log4c logging framework
with elos. The configuration for log4c can be set with the help of a log4crc 
configuration file. 

In order to log with a log4c framework, a category, a priority
and an appender for the logged message should be set in the config file.

.. literalinclude:: ../../../elos/src/demo/log4crc
   :language: xml

More information about how to use log4c framework can be found here : 
https://log4c.sourceforge.net/

