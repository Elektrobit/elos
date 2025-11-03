import robot.utils.asserts
from robot.libraries.BuiltIn import BuiltIn
from robot.api import logger
from robot.api.deco import keyword


class DltTestBuffer(object):

    def __init__(self, **kwargs):
        if "dlt_path" in kwargs:
            self.dlt_path = kwargs["dlt_path"]
        else:
            self.dlt_path = "/dev/shm/test_dlt_shmem"

        if "dlt_offset" in kwargs:
            self.dlt_offset = int(kwargs["dlt_offset"], base=0)
        else:
            self.dlt_offset = 0x1000

        if "dlt_memory_size" in kwargs:
            self.dlt_memory_size = int(kwargs["dlt_memory_size"], base=0)
        elif "dlt_size" in kwargs:
            self.dlt_memory_size = 8 + 144*int(kwargs["dlt_size"], base=0)
        else:
            self.dlt_memory_size = 1448

        if "dlt_size" in kwargs:
            self.dlt_size = int(kwargs["dlt_size"], base=0)
        elif "dlt_memory_size" in kwargs:
            self.dlt_size = (int(kwargs["dlt_memory_size"], base=0) - 8)/144
        else:
            self.dlt_size = 10

        self.ssh = BuiltIn().get_library_instance("SSHLibrary")
        self.is_root = (lambda s: True if s == "True" else False)(
            BuiltIn().get_variable_value("${USER_IS_ROOT}"))
        self.password = BuiltIn().get_variable_value("${PASSWORD}")

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


    def create_dlt_test_buffer(self):
        """
        Create a dlt ring buffer to use for testing the dlt scanner
        """
        stdout, stderr, rc = self._exec_on_target(
            f'mng_dlt_buffer -c {self.dlt_size} -f "{self.dlt_path}" -o "{self.dlt_offset}" -s "{self.dlt_memory_size}" -e'
        )
        if rc != 0:
            raise Exception(stderr)

    def queue_dlt_event(self):
        """
        Queue a fixed predefined event to the dlt buffer
        """
        stdout, stderr, rc = self._exec_on_target(
            f'mng_dlt_buffer -t -f "{self.dlt_path}" -o "{self.dlt_offset}"'
        )
        if rc != 0:
            raise Exception(stderr)

    @keyword("'${number}' Dlt Events Are Queued")
    def some_dlt_events_are_queued(self, number=5):
        """
        Add some dlt events into the shared ring buffer
        """
        for n in range(number):
            self.queue_dlt_event()

