Smoke Tests
===========

Goal:

Smoke tests are quick to execute and they give a brief overview about
the basic functionality. Smoke tests try to answer: Is the software
still basically OK? Or does it crash right away?

The CI system is responsible of running the smoke tests regularly.

Details:

- Runs all composed artifacts to ensure a basic expected functionality
- Make usage of the demo project to check expected behavior of elosd
- Can run against any build type
- Executed by CI-pipeline
- Written in pure shell to run on targets with very limited environment
- Provides a quick response for development
- Provides a interactive environment to experiment from the current build

To run the smoke test from within the source (Developer):
---------------------------------------------------------

First ensure the build is present that shall be tested. Default is asumed
`Debug`.

.. code:: shell

   ./ci/build.sh [Release]

.. program-output:: ./ci/run_smoketests.sh -h
   :shell:

Run on a Target (Integrator):
-----------------------------

To run the smoketest install all content in ``tests/smoketest`` to a
custom location on the target system. Currently it is not packaged and
installed by elos build system itself.

Prerequisites :
~~~~~~~~~~~~~~~

-  Elos is installed on target system

-  Elos smoketest package is installed, see

.. program-output:: cmake -B build/tmp -LH 2>/dev/null | grep -B1 SMOKE_TESTS
   :shell:

-  At least a writeable directory to store results and for intermediate
   files

Running the Test Suite:
~~~~~~~~~~~~~~~~~~~~~~~

To execute the smoketests run the installed smoketest.sh script. Here an
example script to for smoketest integration.

.. code:: shell

    #!/bin/sh -eu

    export PREFIX_PATH=/usr/local/
    export SMOKETEST_RESULT_DIR=/tmp/test-elos-smoketest
    export SMOKETEST_ENABLE_COMPILE_TESTS="yes"
    /usr/local/lib/test/elos/smoketest/smoketest.sh

Extended configuration
~~~~~~~~~~~~~~~~~~~~~~

Depending on the installation and configuration it could be necessary to
adjust some settings. The following Environment variables can be used :

.. program-output:: cat ./test/smoketest/smoketest_env.sh
   :shell:
