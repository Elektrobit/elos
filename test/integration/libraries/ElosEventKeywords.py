# SPDX-License-Identifier: MIT
import json
from types import MappingProxyType
from robot.api.deco import keyword


class ElosEventKeywords(object):
    """
    An elos event class to generate a default elos event, manipulate member variable values
    of default elos events as well as event provided by user
    """

    def __init__(self):
        """
        Initialize class instance by populating member event with defaults.
        """
        self.event = {
            "date": [0, 0],
            "source": {
                "appName": "",
                "fileName": "",
                "pid": 0
            },
            "severity": 0,
            "hardwareid": "",
            "classification": 0,
            "messageCode": 0,
            "payload": ""
        }

        self.event = MappingProxyType(self.event)

    @keyword("Get Default Event")
    def get_default_event(self):
        """
        Returns a copy of the event member as default event.
        """
        default_event = dict(self.event)
        return default_event

    @keyword("Set '${date}' As '${event}' Date")
    def set_event_date(self, date, event):
        """
        Set given date as given event's date.
        """
        event["date"][0] = date
        return event

    @keyword("Set '${app_name}' As '${event}' App Name")
    def set_event_appname(self, app_name, event):
        """
        Set given app name as given event's app name.
        """
        event["source"]["appName"] = app_name
        return event

    @keyword("Set '${file_name}' As '${event}' File Name")
    def set_event_filename(self, file_name, event):
        """
        Set given file name as given event's file name.
        """
        event["source"]["fileName"] = file_name
        return event

    @keyword("Set '${pid}' As '${event}' PID")
    def set_event_pid(self, pid, event):
        """
        Set given pid as given event's pid.
        """
        event["source"]["pid"] = int(pid)
        return event

    @keyword("Set '${severity}' As '${event}' Severity")
    def set_event_severity(self, severity, event):
        """
        Set given severity as given event's severity.
        """
        event["severity"] = int(severity)
        return event

    @keyword("Set '${hardwareid}' As '${event}' Hardwareid")
    def set_event_hardwareid(self, hardwareid, event):
        """
        Set given hardwareid as given event's hardwareid.
        """
        event["hardwareid"] = hardwareid
        return event

    @keyword("Set '${classification}' As '${event}' Classification")
    def set_event_classification(self, classification, event):
        """
        Set given classification as given event's classification.
        """
        event["classification"] = int(classification)
        return event

    @keyword("Set '${message_code}' As '${event}' Message Code")
    def set_event_messagecode(self, message_code, event):
        """
        Set given message code as given event's message code.
        """
        event["messageCode"] = int(message_code)
        return event

    @keyword("Set '${payload}' As '${event}' Payload")
    def set_event_payload(self, payload, event):
        """
        Set given payload as given event's payload.
        """
        event["payload"] = payload
        return event

    @keyword("Get '${event}' As Json String")
    def get_event_json_string(self, event):
        """
        Get given event as json string.
        """
        return json.dumps(event, indent=4)
