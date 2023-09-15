InfluxDB Logging Backend
========================

As an example for an time series database, InfluxDB was chosen in order
to evaluate write performance of time series databases.

Currently, only a Proof-of-Concept implementation exists. The current
implementation lacks configurability, instead opting to use default values.

The current implementation also only uses the old v1 API of InfluxDB, due to
openembedded missing a recipe for more recent versions of InluxDB.

Configuration
-------------
In order to use InfluxDB as a logging backend, the following configuration is
necessary:
	{
	    "root": {
		"elos": {
		    "EventLogging": {
			"PluginSearchPath": "/usr/lib/x86_64-linux-gnu/elos/backend",
			"Plugins": {
			    "InfluxBackend": {
				"File": "backend_influxdb.so",
				"Run": "always",
				"Filter": [
				    "1 1 EQ"
				]
			    }
			}
		    }
		}
	    }
	}



A fully developed InfluxDB Plugin may instead have a config as below:
	{
	    "root": {
		"elos": {
		    "EventLogging": {
			"PluginSearchPath": "/usr/lib/x86_64-linux-gnu/elos/backend",
			"Plugins": {
			    "InfluxBackend": {
				"File": "backend_influxdb.so",
				"Run": "always",
				"Filter": [
				    "1 1 EQ"
				],
				"Config": {
					"Username": "<username>",
					"Password": "<password>",
					"Token": "<token>",
				}
			    }
			}
		    }
		}
	    }
	}

There is no configuration for the storage location, since this will be
configured in the InfluxDB configuration.

While it is planned to make the password and token both configurable in the file,
it is advisable to instead use environment variables to configure both values.

It also only necessary to configure either Username and Password or the Token,
since both fullfill the same goal of authenticating elosd to the database.

Database
--------
Elosd will store its events in the InfluxDB database, or "bucket" 'elosd', and
will store the events in the table, or "measurement" 'event'.

The event-fields 'soruce', with its values and 'hardware-id' are used as tags
for the time series database, while the fields 'severity', 'classification',
'messageCode' and 'payload' are used as 'fields', meaning they are fields that
Influx expects to vary a lot.
