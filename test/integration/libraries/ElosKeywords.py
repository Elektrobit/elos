# SPDX-License-Identifier: MIT
import time
import json
import uuid
import re
from urllib.parse import urlparse
import robot.utils.asserts
from robot.libraries.BuiltIn import BuiltIn
from robot.api import logger
from robot.api.deco import keyword


def string_start_with(string, pattern):
    match = re.match(pattern, string)
    if match:
        return True
    else:
        return False


class ElosSubscription(object):
    """
    Manage subscription process though ssh and elosc. Each instance starts a
    single elosc process subscribing to a given filter in the background.
    """

    def __init__(self, filter, connection_setup):
        self.ssh = BuiltIn().get_library_instance('SSHLibrary')
        self.subID = uuid.uuid1()
        self.connectionAlias = f"Subscription{self.subID}"
        self.subscription_out = f"/tmp/subscription{self.subID}.stdout"
        self.subscription_err = f"/tmp/subscription{self.subID}.stderr"
        self.subscription_pid = f"/tmp/subscription{self.subID}.pid"

        connectionToRestore = self.ssh.get_connection().index
        host = BuiltIn().get_variable_value('${TARGET_IP}')
        port = 22
        user = BuiltIn().get_variable_value('${TARGET_USER}')
        password = BuiltIn().get_variable_value('${TARGET_PASSWORD}')
        self.connectionId = self.ssh.open_connection(host,
                                                     self.connectionAlias,
                                                     port)
        self.ssh.login(user, password)

        self.ssh.start_command(
            f"{connection_setup} -s \"{filter}\" > {self.subscription_out}"
            f" 2> {self.subscription_err} &"
            f" echo $! > {self.subscription_pid}")

        self.ssh.switch_connection(connectionToRestore)

    def get_errors(self):
        """
        Read and parse the elosc errorsi buufer and return all errors occured
        in this subscription.
        """
        connectionToRestore = self.ssh.switch_connection(self.connectionAlias)
        stdout, stderr, rc = self.ssh.execute_command(
            f"cat {self.subscription_err}",
            return_stdout=True,
            return_stderr=True,
            return_rc=True)
        self.ssh.switch_connection(connectionToRestore)

        if rc != 0 or stderr:
            logger.error(
                f"Failed to fetch subscriptions stderr  ({rc}): {stderr}")
            errors = []
        else:
            errors = stdout.splitlines()

        logger.info(f"Following errors found : {errors}")
        return errors

    def get_events(self):
        """
        Read and parse the elosc output and return all events received
        by this subscription.
        """
        connectionToRestore = self.ssh.switch_connection(self.connectionAlias)
        stdout, stderr, rc = self.ssh.execute_command(
            f"cat {self.subscription_out}",
            return_stdout=True,
            return_stderr=True,
            return_rc=True)
        self.ssh.switch_connection(connectionToRestore)

        if rc != 0 or stderr:
            logger.error(f"Failed to fetch subscriptions ({rc}): {stderr}")
            events = []
        else:
            logger.info(f"Contents of stdout: {stdout}")
            events = self._parse_elosc_result(stdout)
            logger.info(f"parsed {len(events)} events from {events}")

        return events

    def _parse_elosc_result(self, stdout):
        """
        Parse elosc console output for subscriptions
        """
        lines = stdout.splitlines()
        events = []
        for i, line in enumerate(lines):
            pattern = r'^new data \[(\d+),(\d+)\]:$'
            if string_start_with(line.strip(), pattern) and len(lines) > i + 1:
                events.append(json.loads(lines[i + 1].strip()))
        return events

    def unsubscribe(self):
        """
        Kill and cleanup elosc subscribe process owned by this instance.
        """
        connectionToRestore = self.ssh.switch_connection(self.connectionAlias)
        stdout, stderr = self.ssh.execute_command(
            f"kill $(cat {self.subscription_pid})",
            return_stdout=True,
            return_stderr=True,
        )
        if stdout:
            logger.info(f"Unsubscribe contents of stdout: {stdout}")
        if stderr:
            logger.error(f"Unsubscribe contents of stderr: {stderr}")

        stdout, stderr = self.ssh.execute_command(
            (f"rm {self.subscription_pid}"
             f" {self.subscription_out}"
             f" {self.subscription_err}"),
            return_stdout=True,
            return_stderr=True,
        )
        if stdout:
            logger.info(f"Unsubscribe contents of stdout: {stdout}")
        if stderr:
            logger.error(f"Unsubscribe contents of stderr: {stderr}")
        self.ssh.switch_connection(connectionToRestore)


class ElosKeywords(object):

    def __init__(self):
        self.ssh = BuiltIn().get_library_instance("SSHLibrary")
        self.is_root = (lambda s: True if s == "True" else False)(
            BuiltIn().get_variable_value("${USER_IS_ROOT}"))
        self.password = BuiltIn().get_variable_value("${PASSWORD}")
        self.status_command = BuiltIn().get_variable_value(
            "${ELOS_STATUS_COMMAND}")
        self.start_command = BuiltIn().get_variable_value(
            "${ELOS_START_COMMAND}")
        self.stop_command = BuiltIn().get_variable_value(
            "${ELOS_STOP_COMMAND}")
        self.publish_time = 0

    def _exec_on_target(self, cmd, err_msg=None):
        if err_msg is None:
            err_msg = cmd

        stdout, stderr, rc = self.ssh.execute_command(
            cmd,
            return_stdout=True,
            return_stderr=True,
            return_rc=True,
            sudo=(not self.is_root),
            sudo_password=(None if self.is_root else self.password),
        )

        if rc != 0 and stderr:
            logger.debug(f"exec on target: {err_msg} ({rc}): {stderr}")
        elif rc != 0:
            logger.debug(f"exec on target: {err_msg} ({rc})")

        return stdout, stderr, rc

    def _get_elosd_pid(self):
        pid = -1
        stdout, stderr, rc = self._exec_on_target("pgrep elosd")
        if rc == 0:
            pid = int(stdout)
        return pid

    def _get_elosd_status(self):
        stdout, stderr, rc = self._exec_on_target(self.status_command)

        logger.info(f"{rc}: {stdout}, {stderr}")

        return "running" in stdout.lower()

    def _get_connection_setup(self, uri):
        parsed_uri = urlparse(uri)

        if parsed_uri.scheme == "unix":
            setup = "elosc -U " + parsed_uri.path
        elif parsed_uri.scheme == "tcp":
            setup = "elosc -H " + parsed_uri.hostname + " -P " + str(
                parsed_uri.port)
        else:
            robot.utils.asserts.fail(
                f"Connection type not supported {parsed_uri.scheme}")
        return setup

    def _start_elosd(self):
        stdout, stderr, rc = self._exec_on_target(self.start_command)

        return rc == 0

    def _stop_elosd(self):
        stdout, stderr, rc = self._exec_on_target(self.stop_command)

        return rc == 0

    @keyword("Get Current Time From Target")
    def get_current_time_from_target(self):
        """
        Get the current time from test target
        """

        stdout, stderr, rc = self._exec_on_target("date +%s")
        robot.utils.asserts.assert_true(rc == 0)
        return stdout
        
    def _set_publish_time(self, event):
        eventd = json.loads(event)
        if eventd.get("date") is None or eventd["date"][0] == 0:
            self.publish_time = int(self.get_current_time_from_target())
        else:
            self.publish_time = int(eventd["date"][0])

        logger.info(f"publish time : {self.publish_time}")

    def start_elosd(self):
        """
        Start elosd on target using ELOS_START_COMMAND.
        """

        status = self._start_elosd()
        robot.utils.asserts.assert_true(status)

    def stop_elosd(self):
        """
        Stop elosd on target using ELOS_STOP_COMMAND.
        """

        status = self._stop_elosd()
        robot.utils.asserts.assert_true(status)

    def restart_elosd(self):
        """
        stop and start an instance of elosd.
        """

        self.stop_elosd()
        self.wait_for_elosd_to_stop()
        self.start_elosd()
        self.wait_till_elosd_is_started()

    @keyword("Publish '${event}'")
    def publish_event(self, event):
        """
        publish an event on target
        """
        stdout, stderr, rc = self.publish_event_via(event,
                                                    "tcp://127.0.0.1:54321")
        return stdout, stderr, rc

    @keyword("Publish '${event}' Via '${uri}'")
    def publish_event_via(self, event, uri):
        """
        publish an event via given connection string
        """
        connection_setup = self._get_connection_setup(uri)
        self._set_publish_time(event)
        stdout, stderr, rc = self._exec_on_target(
            f"{connection_setup} -p '{event}'")
        return stdout, stderr, rc

    @keyword("Publish '${event}' '${count}' Times")
    def publish_event_x_times(self, event, count):
        """
        publish an event on target given number of times
        note: publish time will be set to time before first event publish
        """
        stdout, stderr, rc = self.publish_event_x_times_via(
            event, count, "tcp://127.0.0.1:54321")
        return stdout, stderr, rc

    @keyword("Publish '${event}' '${count}' Times Via '${uri}'")
    def publish_event_x_times_via(self, event, count, uri):
        """
        publish an event on target given number of times via given
        connection string
        note: publish time will be set to time before first event publish
        """
        stdout = []
        stderr = []
        rc = []
        self._set_publish_time(event)
        for i in range(int(count)):
            out, err, ret = self.publish_event_via(event, uri)
            logger.info(f"published event : {out}")
            stdout.append(out)
            stderr.append(err)
            rc.append(ret)

        return stdout, stderr, rc

    @keyword("Find Events Matching '${filter}'")
    def find_events_matching(self, filter):
        """
        find events matching given event filter via default uri
        """

        stdout, stderr, rc = self.find_events_matching_via(
            filter, "tcp://127.0.0.1:54321")

        return stdout, stderr, rc

    @keyword("Find Events Matching '${filter}' Via '${uri}'")
    def find_events_matching_via(self, filter, uri):
        """
        find events matching given event filter via given uri
        """

        connection_setup = self._get_connection_setup(uri)
        stdout, stderr, rc = self._exec_on_target(
            f"{connection_setup} -f '{filter}'")

        return stdout, stderr, rc

    def wait_for_elosd_to_stop(self, timeout=30):
        """
        Wait for a running elosd instance to stop. Fail if timeout is reached.
        Params:
            timeout: wait for seconds to fail.
        """

        is_stopped = not self._get_elosd_status()

        start_time = time.time()
        while not is_stopped:
            if time.time() - start_time > timeout:
                break
            time.sleep(0.2)
            is_stopped = not self._get_elosd_status()

        robot.utils.asserts.assert_true(is_stopped)

    def wait_till_elosd_is_started(self, timeout=30):
        """
        Wait till elosd is started and running. Fail if timeout is reached.
        Params:
            timeout: wait for seconds to fail.
        """

        is_started = self._get_elosd_status()

        start_time = time.time()
        while not is_started:
            if time.time() - start_time > timeout:
                break
            time.sleep(0.2)
            is_started = self._get_elosd_status()

        robot.utils.asserts.assert_true(is_started)

    def is_elosd_listen_on(self, port=54321):
        port_as_hex = hex(port).upper()[2:]  # trim leading '0X'
        pid = self._get_elosd_pid()
        _, _, rc = self._exec_on_target(
            f"grep '{port_as_hex}' /proc/{pid}/net/tcp")
        return rc == 0

    def wait_till_elosd_is_listening_on(self, port=54321, timeout=30):
        is_listening = self.is_elosd_listen_on(port)
        start_time = time.time()
        while not is_listening:
            if time.time() - start_time > timeout:
                break
            time.sleep(0.2)
            is_listening = self.is_elosd_listen_on(port)
        robot.utils.asserts.assert_true(is_listening)

    def ensure_elosd_is_started(self):
        """
        Check if elosd is running and start elosd using `Start Elosd` keyword
        if not running. Fail if elosd can't be started.
        """

        status = self._get_elosd_status()
        if status is not True:
            status = self._start_elosd()
            robot.utils.asserts.assert_true(status)
            self.wait_till_elosd_is_started()
            status = self._get_elosd_status()
        robot.utils.asserts.assert_true(status)

    def ensure_elosd_is_stopped(self):
        """
        Check if elosd is stopped and stop elosd using `Stop Elosd` keyword
        if still running. Fail if elosd can't be stopped.
        """

        is_stopped = not self._get_elosd_status()
        if is_stopped is not True:
            self._stop_elosd()
            self.wait_for_elosd_to_stop()
            is_stopped = not self._get_elosd_status()
        robot.utils.asserts.assert_true(is_stopped)

    def _parse_elosc_result(self, stdout):
        lines = stdout.splitlines()
        events = []
        for i, line in enumerate(lines):
            if line.strip() == "new data:" and len(lines) > i + 1:
                events.append(json.loads(lines[i + 1].strip()))
        return events

    def ensure_x_events_are_stored_with_payload(self,
                                                filter,
                                                count,
                                                payload,
                                                timeout=10):
        """
        Continuously fetch elos events for a given filter and payload until
        either expected amount is seen or a timeout occurred via default 
        connection.
        """
        self.ensure_x_events_are_stored_with_payload_via(
            filter, count, payload, "tcp://127.0.0.1:54321")

    def ensure_x_events_are_stored_with_payload_via(self,
                                                    filter,
                                                    count,
                                                    payload,
                                                    uri,
                                                    timeout=10):
        """
        Continuously fetch elos events for a given filter and payload until
        either expected amount is seen or a timeout occurred via given connection.
        """

        start_time = time.time()
        expected_count = int(count)
        retry_count = 0
        eventCount = -1
        while True:
            stdout, stderr, rc = self.find_events_matching_via(filter, uri)
            if rc != 0:
                robot.utils.asserts.fail()

            events = self._parse_elosc_result(stdout)
            logger.info(f"found {len(events)} events"
                        f" look for payload like '{payload}'")
            filtered_events = []
            for event in events:
                if event.get("payload").find(payload) != -1:
                    filtered_events.append(event)

            eventCount = len(filtered_events)
            logger.info(f"found {eventCount} events with payload")
            if expected_count == eventCount:
                break
            elif time.time() - start_time > timeout:
                logger.info("elosc output was:")
                logger.info(f"stdout:\n{stdout}\nstderr:\n{stderr}\n")
                logger.info("Fail because of timeout")
                robot.utils.asserts.assert_equal(expected_count, eventCount)
            else:
                logger.info("elosc output was:")
                logger.info(f"stdout:\n{stdout}\nstderr:\n{stderr}\n")
                retry_count += 1
                logger.info(f"{retry_count}. Retry as not enough events found")
                time.sleep(0.2)

    def get_events(self, event_string):
        """
        Get events from parsing event_string
        """

        logger.info(f"get events from : {event_string}")
        events = self._parse_elosc_result(event_string)

        return events

    def _matching_events_are_current(self, filter, uri):
        matched_event_count = 0
        matched_events = self.get_events(
            self.find_events_matching_via(filter, uri)[0])
        if matched_events:
            for event in matched_events:
                if event.get("date")[0] >= self.publish_time:
                    matched_event_count += 1

        logger.info(f"found {matched_event_count} after publish time")

        return matched_event_count, matched_event_count > 0

    @keyword("Latest Events Matching '${filter}' Found")
    def latest_events_matching_are_found(self, filter):
        """
        Assert if events matching filter are found and are latest via default connection
        """
        self.latest_events_matching_are_found_via(filter,
                                                  "tcp://127.0.0.1:54321")

    @keyword("Latest Events Matching '${filter}' Found Via '${uri}'")
    def latest_events_matching_are_found_via(self, filter, uri):
        """
        Assert if events matching filter are found and are latest via given connection
        """
        count, status = self._matching_events_are_current(filter, uri)
        robot.utils.asserts.assert_true(status)

    @keyword("Latest Events Matching '${filter}' Not Found")
    def latest_events_matching_are_not_found(self, filter):
        """
        Assert if events matching filter are not found via default connection
        """
        self.latest_events_matching_are_not_found_via(filter,
                                                      "tcp://127.0.0.1:54321")

    @keyword("Latest Events Matching '${filter}' Not Found Via '${uri}'")
    def latest_events_matching_are_not_found_via(self, filter, uri):
        """
        Assert if events matching filter are not found via given connection
        """
        count, status = self._matching_events_are_current(filter, uri)
        robot.utils.asserts.assert_false(status)

    @keyword("'${count}' Latest Events Matching '${filter}' Found")
    def X_latest_events_matching_are_found(self, count, filter):
        """
        Assert if events matching filter equal given count via default connection 
        """
        self.X_latest_events_matching_are_found_via(count, filter,
                                                    "tcp://127.0.0.1:54321")

    @keyword("'${count}' Latest Events Matching '${filter}' Found Via '${uri}'"
             )
    def X_latest_events_matching_are_found_via(self, count, filter, uri):
        """
        Assert if events matching filter equal given count via given connection 
        """
        matched_event_count, status = self._matching_events_are_current(
            filter, uri)
        robot.utils.asserts.assert_equal(int(count), matched_event_count)
        robot.utils.asserts.assert_true(status)

    @keyword("Directory '${path}' Should Exist On Target")
    def directory_exists_on_target(self, path):
        stdout, stderr, rc = self._exec_on_target(f"test -d {path}")
        if rc != 0:
            robot.utils.asserts.fail(f"Directory {path} does not exist")

    @keyword("File '${path}' Should Exist On Target")
    def file_exists_on_target(self, path):
        stdout, stderr, rc = self._exec_on_target(f"test -f {path}")
        if rc != 0:
            robot.utils.asserts.fail(f"File {path} does not exist")

    @keyword("'${plugin}' Plugin Is Loaded")
    def plugin_is_loaded(self, plugin):
        """
        Verify that a plugin with given name is reported as loaded by elos.
        """
        stdout, stderr, rc = self._exec_on_target(
            f"grep -i \"'{plugin}' has been loaded\" /tmp/elosd.log")
        if rc != 0:
            robot.utils.asserts.fail(f"Plugin {plugin} not loaded")

    @keyword("Write '${message}' To '${file}'")
    def write_to_file(self, message, file):
        """
        Append a given string to given file on the target.
        """
        stdout, stderr, rc = self._exec_on_target(
            f"sh -c 'echo \"{message}\" >> \"{file}\"'")
        if rc != 0:
            robot.utils.asserts.fail(f"Could not write to {file}")

    @keyword("Subscribe To '${filter}'")
    def subscribe_to_event(self, filter):
        """
        Create a subscription for a given event filter using
        default connection string
        """
        return self.subscribe_to_event_via(filter, "tcp://127.0.0.1:54321")

    @keyword("Subscribe To '${filter}' Via '${uri}'")
    def subscribe_to_event_via(self, filter, uri):
        """
        Create a subscription for a given event filter
        """
        logger.info(f"subscribe to {filter} via {uri}")
        connection_setup = self._get_connection_setup(uri)
        subscription = ElosSubscription(filter, connection_setup)
        BuiltIn().set_test_variable('${SUBSCRIPTION}', subscription)
        return subscription

    @keyword("Subscription Should Fail")
    def subscription_fails(self, timeout=5):
        """
        Check client subscription for failure
        """
        subscription_error = "event subscription failed"
        subscription = BuiltIn().get_variable_value("${SUBSCRIPTION}")
        start_time = time.time()
        retry_count = 0
        while True:
            errors = subscription.get_errors()
            status = subscription_error in errors
            if status:
                break
            elif time.time() - start_time > timeout:
                robot.utils.asserts.fail("Fail because of timeout")
            else:
                retry_count += 1
                logger.info(f"{retry_count}. Retry as no events found")
                time.sleep(0.2)

        robot.utils.asserts.assert_true(
            status, "subscription does not fail as expected")

    @keyword("Unsubscribe from ${subscription}")
    def unsubscribe_from(self, subscription):
        """
        Cancel a subscription client and clean everything up
        """
        if not subscription:
            robot.utils.asserts.fail("No active subscription to unsubscribe")
        else:
            subscription.unsubscribe()

    @keyword("Unsubscribe All")
    def unsubscribe_all(self):
        """
        Cancel all active subscriptions
        """
        subscription = BuiltIn().get_variable_value("${SUBSCRIPTION}")
        self.unsubscribe_from(subscription)

    @keyword("Client Receives Event Subscribed To")
    def client_receives_event_subscribed_to(self, timeout=5):
        """
        Check last subscription has received events subscribed to
        """
        subscription = BuiltIn().get_variable_value("${SUBSCRIPTION}")
        logger.info(f"subID: {subscription.subID}")
        start_time = time.time()
        retry_count = 0
        while True:
            events = subscription.get_events()
            if len(events) > 0:
                logger.info(f"{events}")
                break
            elif time.time() - start_time > timeout:
                robot.utils.asserts.fail("Fail because of timeout")
            else:
                retry_count += 1
                logger.info(f"{retry_count}. Retry as no events found")
                time.sleep(0.2)

    @keyword("Client With '${subscription}' Receives Event")
    def client_with_subscription_receives_event(self, subscription, timeout=5):
        """
        Check event with given subscription has received events subscribed to
        """
        logger.info(f"subID: {subscription.subID}")
        start_time = time.time()
        retry_count = 0
        while True:
            events = subscription.get_events()
            if len(events) > 0:
                logger.info(f"{events}")
                break
            elif time.time() - start_time > timeout:
                robot.utils.asserts.fail("Fail because of timeout")
            else:
                retry_count += 1
                logger.info(f"{retry_count}. Retry as no events found")
                time.sleep(0.2)
