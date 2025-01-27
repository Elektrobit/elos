# SPDX-License-Identifier: MIT

from robot.api import logger
from robot.libraries.BuiltIn import BuiltIn
import os

class TestListener:
    ROBOT_LISTENER_API_VERSION = 3

    def end_test(self, data, result):
        self._copy_test_log(result)

    def _copy_test_log(self, test_result):
        try:
            test_output_path = os.getenv("TEST_OUTPUT", "/base/build/Debug/result/integration")
            if not test_result.passed:
                log_path="logs/fail"
            else:
                log_path="logs/pass"
            test_file_path = os.path.join(test_output_path, log_path, f"{test_result.name}.log")
            log_file_path = BuiltIn().get_variable_value("${ELOSD_LOG}")
            if not log_file_path:
                raise ValueError("Failed to retrieve ${ELOSD_LOG}")
            ssh = BuiltIn().get_library_instance('SSHLibrary')
            ssh.get_file(log_file_path, test_file_path)
        except Exception as e:
            logger.error(f"Failed to copy log for '{test_result.name}': {e}")


