# SPDX-License-Identifier: MIT
from distutils.spawn import find_executable

class OsLibrary(object):
    """OsLibrary provides a collection of keywords for communicating with OS
    environment. """

    def program_exists(self, program):
        """Check if given program exists.

        | program | To be checked |
        """
        return find_executable(program) is not None
