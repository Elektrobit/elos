import time
import json
import re
import robot.utils.asserts
from robot.libraries.BuiltIn import BuiltIn
from robot.api import logger
from robot.api.deco import keyword


class ElosKeywords(object):

    def __init__(self):
        self.ssh = BuiltIn().get_library_instance('SSHLibrary')
        self.is_root = (lambda s: True if s == "True" else False)(
            BuiltIn().get_variable_value('${USER_IS_ROOT}'))
        self.password = BuiltIn().get_variable_value('${PASSWORD}')
        self.status_command = BuiltIn().get_variable_value(
            '${ELOS_STATUS_COMMAND}')
        self.start_command = BuiltIn().get_variable_value(
            '${ELOS_START_COMMAND}')
        self.stop_command = BuiltIn().get_variable_value(
            '${ELOS_STOP_COMMAND}')
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
            sudo_password=(None if self.is_root else self.password)
        )

        if rc != 0 and stderr:
            logger.error(f"exec on target: {err_msg} ({rc}): {stderr}")
        elif rc != 0:
            logger.error(f"exec on target: {err_msg} ({rc})")

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

    def _start_elosd(self):
        stdout, stderr, rc = self._exec_on_target(self.start_command)

        return rc == 0

    def _stop_elosd(self):
        stdout, stderr, rc = self._exec_on_target(self.stop_command)

        return rc == 0

    def _set_publish_time(self):
        stdout, stderr, rc = self._exec_on_target("date +%s")
        if rc == 0:
            self.publish_time = int(stdout)

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

    def set_event_publish_time(self, event):
        self._set_publish_time()
        new_event = re.sub(re.escape("ptime"), str(self.publish_time), event, count=1)
        logger.info(f"event with pub time : {new_event}")

        return new_event

    def publish_event(self, event, port=54321):
        """
        publish an event on target
        """

        stdout, stderr, rc = self._exec_on_target(f"elosc -P '{port}' -p '{event}'")

        return rc == 0

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
            if line.strip() == 'new data:' and len(lines) > i+1:
                events.append(json.loads(lines[i + 1].strip()))
        return events

    def ensure_x_events_are_stored_with_payload(self, filter, count, payload,
                                                timeout=10):
        """
        Continuously fetch elos events for a given filter and payload until
        either expected amount is seen or a timeout occurred.
        """

        start_time = time.time()
        expected_count = int(count)
        retry_count = 0
        eventCount = -1
        while True:
            stdout, stderr, rc = self._exec_on_target(f"elosc -f '{filter}'")
            if rc != 0:
                robot.utils.asserts.fail()

            events = self._parse_elosc_result(stdout)
            logger.info(
                f"found {len(events)} events"
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

    def find_events_matching(self, filter, port=54321):
        """
        find an event published on target
        """

        stdout, stderr, rc = self._exec_on_target(f"elosc -P '{port}' -f '{filter}'")
        logger.info(f"filter run : {stdout}")
        if rc != 0:
                robot.utils.asserts.fail()

        events = self._parse_elosc_result(stdout)

        return events

    def _matching_events_are_current(self, filter):
        matched_event_count = 0;
        matched_events = self.find_events_matching(filter)
        if matched_events:
            for event in matched_events:
                if event.get("date")[0] >= self.publish_time:
                    matched_event_count += 1

        logger.info(f"found {matched_event_count} after publish time")

        return matched_event_count > 0

    @keyword("Latest Events Matching ${filter} Found")
    def latest_events_matching_are_found(self, filter):
        status = self._matching_events_are_current(filter)
        robot.utils.asserts.assert_true(status)

    @keyword("Latest Events Matching ${filter} Not Found")
    def latest_events_matching_are_not_found(self, filter):
        status = self._matching_events_are_current(filter)
        robot.utils.asserts.assert_false(status)

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
