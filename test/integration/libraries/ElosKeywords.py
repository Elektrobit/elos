import robot.utils.asserts
import time
from robot.libraries.BuiltIn import BuiltIn
from robot.api import logger


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

    def ensure_elosd_is_started(self):
        """
        Check if elosd is running and start elosd using `Start Elosd` keyword
        if not running. Fail if elosd can't be started.
        """

        status = self._get_elosd_status()
        if status is not True:
            self._start_elosd()
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
